/*
 * Copyright (c) 1987 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
static char sccsid[] = "@(#)addbytes.c	5.10 (Berkeley) 09/21/92";
#endif	/* not lint */

#include <curses.h>
#include <termios.h>

#define	SYNCH_IN	{y = win->cury; x = win->curx;}
#define	SYNCH_OUT	{win->cury = y; win->curx = x;}

/*
 * waddbytes --
 *	Add the character to the current position in the given window.
 */
int
waddbytes(win, bytes, count)
	register WINDOW *win;
	register char *bytes;
	register int count;
{
	static char blanks[] = "        ";
	register int c, newx, x, y;
	LINE *lp;

	SYNCH_IN;

#ifdef DEBUG
	__TRACE("ADDBYTES('%c') at (%d, %d)\n", c, y, x);
#endif
	while (count--) {
		c = *bytes++;
		switch (c) {
		case '\t':
			SYNCH_OUT;
			if (waddbytes(win, blanks, 8 - (x % 8)) == ERR)
				return (ERR);
			SYNCH_IN;
			break;

		default:
			if (win->flags & __WSTANDOUT)
				c |= __STANDOUT;
#ifdef DEBUG
	__TRACE("ADDBYTES(%0.2o, %d, %d)\n", win, y, x);
#endif
			
			lp = win->lines[y];
			if (lp->flags & __ISPASTEOL) {
				lp->flags &= ~__ISPASTEOL;
newline:			if (y == win->maxy - 1) {
					if (win->flags & __SCROLLOK) {
					        x = 0;
						SYNCH_OUT;
						scroll(win);
						SYNCH_IN;
						lp = win->lines[y];
					} 
				} else {
					y++;
					lp = win->lines[y];
					x = 0;
				}
			}
				
							
#ifdef DEBUG
	__TRACE("ADDBYTES: 1: y = %d, x = %d, firstch = %d, lastch = %d\n",
	    y, x, win->lines[y]->firstch, win->lines[y]->lastch);
#endif
			if (lp->line[x] != c) {
				newx = x + win->ch_off;
				if (!(lp->flags & __ISDIRTY)) {
					lp->flags |= __ISDIRTY;
					lp->firstch = lp->lastch = newx;
				}
				else if (newx < lp->firstch)
					lp->firstch = newx;
				else if (newx > lp->lastch)
					lp->lastch = newx;
#ifdef DEBUG
	__TRACE("ADDBYTES: change gives f/l: %d/%d [%d/%d]\n",
	    lp->firstch, lp->lastch,
	    lp->firstch - win->ch_off,
	    lp->lastch - win->ch_off);
#endif
			}
			lp->line[x] = c;
			if (x == win->maxx - 1)
				lp->flags |= __ISPASTEOL;
			else
				x++;
#ifdef DEBUG
	__TRACE("ADDBYTES: 2: y = %d, x = %d, firstch = %d, lastch = %d\n",
	    y, x, win->lines[y]->firstch, win->lines[y]->lastch);
#endif
			break;
		case '\n':
			SYNCH_OUT;
			wclrtoeol(win);
			SYNCH_IN;
			if (origtermio.c_oflag & ONLCR)
				x = 0;
			goto newline;
		case '\r':
			x = 0;
			break;
		case '\b':
			if (--x < 0)
				x = 0;
			break;
		}
	}
	SYNCH_OUT;
	return (OK);
}
 
