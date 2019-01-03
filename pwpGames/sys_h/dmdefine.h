#define DBlack Black	/* No longer (4/25) reversed until window routines set up H/W */
#define DWhite White

#ifdef CRTD

#define DFont1 COURIER12
#define DMPW		6/6					/* Formula for converting LCD 6 wide to CRT 8 wide */
#define DMPH		9/9					/* The original pixels were done for LCD */
#define CHAR_W		8					/* Formula for converting LCD 6 wide to CRT 8 wide */
#define CHAR_H		17					/* The original pixels were done for LCD */


#else

#define DFont1 COURIER10
#define DMPW		6/6					/* Formula for converting LCD 6 wide to CRT 8 wide */
#define DMPH		9/9					/* The original pixels were done for LCD */
#define CHAR_W		6					/* Formula for converting LCD 6 wide to CRT 8 wide */
#define CHAR_H		9					/* The original pixels were done for LCD */

#endif

#define N_KEY  'n'		/* Temporary - suggest SELECT+N for internationals */
#define Y_KEY  'y'

#define CODRET_KEY CODE_KEY+RETURN_KEY
#define CODEDEL_KEY CODE_KEY+DELETE_KEY

#define SELCURL_KEY  SELECT_KEY+CURSLEFT_KEY
#define SELCURR_KEY  SELECT_KEY+CURSRIGHT_KEY

#define SEL_D_KEY SELECT_KEY+'d'		/* Left and Right icon bars */
#define SEL_P_KEY SELECT_KEY+'p'
#define SEL_I_KEY SELECT_KEY+'i'
#define SEL_C_KEY SELECT_KEY+'c'
#define SEL_A_KEY SELECT_KEY+'a'
#define SEL_F_KEY SELECT_KEY+'f'
#define SEL_U_KEY SELECT_KEY+'u'
#define SEL_R_KEY SELECT_KEY+'r'
#define SEL_L_KEY SELECT_KEY+'l'
#define SEL_E_KEY SELECT_KEY+'e'

