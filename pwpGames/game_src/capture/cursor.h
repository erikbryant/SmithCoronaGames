/*************************************************************************
** Module Name:
**
**    cursor
**
** Description:
**
**    This is the header file for the cursor module
**
** Change Log:
**
**    Date           Description
**    ----           -----------
**
**    04/06/92       Creation date
**
**************************************************************************/

void  move_cursor  (UINT2 dir,INT1 *x,INT1 *y);
void  erase_cursor (INT1 x, INT1 y);
void  draw_cursor  (INT1 x, INT1 y);
void  drawmarker     (INT2 x, INT2 y);
void  erasemarker    (INT2 x, INT2 y);

