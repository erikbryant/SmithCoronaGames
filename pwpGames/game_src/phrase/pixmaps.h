/*****************************************************************************
**
**  Module Name:
**      Spin-A-Phrase
**  
**  Description:
**      This include file defines all the pixmaps used by Spin-A-Phrase.
**
**  Exported Units:
**      none
**
**  Local Units:
**      none
**      
**  Exported Data:
**
**  Notes:
**      none
**
**  Change Log:
**      Date            Description of Change
**      ----            ---------------------
**      05/20/92        Version 1.0 
**
**
*****************************************************************************/

/*
** These are the included PIXMAPs depicting slots on the wheel.
** Each slot has an upper and lower component.
*/

#ifdef LCD
   PIXMAP Pix_one_hun_upper[] =
   {
      #include "d100u.pix"
   };

   PIXMAP Pix_one_hun_lower[] =
   {
      #include "d100l.pix"
   };

   PIXMAP Pix_two_hun_upper[] =
   {
      #include "d200u.pix"
   };

   PIXMAP Pix_two_hun_lower[] =
   {
      #include "d200l.pix"
   };

   PIXMAP Pix_three_hun_upper[] =
   {
      #include "d300u.pix"
   };

   PIXMAP Pix_three_hun_lower[] =
   {
      #include "d300l.pix"
   };

   PIXMAP Pix_four_hun_upper[] =
   {
      #include "d400u.pix"
   };

   PIXMAP Pix_four_hun_lower[] =
   {
      #include "d400l.pix"
   };

   PIXMAP Pix_five_hun_upper[] =
   {
      #include "d500u.pix"
   };

   PIXMAP Pix_five_hun_lower[] =
   {
      #include "d500l.pix"
   };

   PIXMAP Pix_six_hun_upper[] =
   {
      #include "d600u.pix"
   };

   PIXMAP Pix_six_hun_lower[] =
   {
      #include "d600l.pix"
   };

   PIXMAP Pix_sev_hun_upper[] =
   {
      #include "d700u.pix"
   };

   PIXMAP Pix_sev_hun_lower[] =
   {
      #include "d700l.pix"
   };

   PIXMAP Pix_eight_hun_upper[] =
   {
      #include "d800u.pix"
   };

   PIXMAP Pix_eight_hun_lower[] =
   {
      #include "d800l.pix"
   };

   PIXMAP Pix_nine_hun_upper[] =
   {
      #include "d900u.pix"
   };

   PIXMAP Pix_nine_hun_lower[] =
   {
      #include "d900l.pix"
   };

   PIXMAP Pix_thousand_upper[] =
   {
      #include "d1000u.pix"
   };

   PIXMAP Pix_thousand_lower[] =
   {
      #include "d1000l.pix"
   };

   PIXMAP Pix_fifteen_hun_upper[] =
   {
      #include "d1500u.pix"
   };

   PIXMAP Pix_fifteen_hun_lower[] =
   {
      #include "d1500l.pix"
   };

   PIXMAP Pix_free_upper[] =
   {
      #include "dfreeu.pix"
   };

   PIXMAP Pix_free_lower[] =
   {
      #include "dfreel.pix"
   };

   PIXMAP Pix_lose_turn_upper[] =
   {
      #include "dloseu.pix"
   };

   PIXMAP Pix_lose_turn_lower[] =
   {
      #include "dlosel.pix"
   };

   PIXMAP Pix_broke_upper[] =
   {
      #include "dbrokeu.pix"
   };

   PIXMAP Pix_broke_lower[] =
   {
      #include "dbrokel.pix"
   };

   PIXMAP Arrow[] =
   {
      #include "darrow.pix"
   };
#else
   PIXMAP Pix_one_hun_upper[] =
   {
      #include "100u.pix"
   };

   PIXMAP Pix_one_hun_lower[] =
   {
      #include "100l.pix"
   };

   PIXMAP Pix_two_hun_upper[] =
   {
      #include "200u.pix"
   };

   PIXMAP Pix_two_hun_lower[] =
   {
      #include "200l.pix"
   };

   PIXMAP Pix_three_hun_upper[] =
   {
      #include "300u.pix"
   };

   PIXMAP Pix_three_hun_lower[] =
   {
      #include "300l.pix"
   };

   PIXMAP Pix_four_hun_upper[] =
   {
      #include "400u.pix"
   };

   PIXMAP Pix_four_hun_lower[] =
   {
      #include "400l.pix"
   };

   PIXMAP Pix_five_hun_upper[] =
   {
      #include "500u.pix"
   };

   PIXMAP Pix_five_hun_lower[] =
   {
      #include "500l.pix"
   };

   PIXMAP Pix_six_hun_upper[] =
   {
      #include "600u.pix"
   };

   PIXMAP Pix_six_hun_lower[] =
   {
      #include "600l.pix"
   };

   PIXMAP Pix_sev_hun_upper[] =
   {
      #include "700u.pix"
   };

   PIXMAP Pix_sev_hun_lower[] =
   {
      #include "700l.pix"
   };

   PIXMAP Pix_eight_hun_upper[] =
   {
      #include "800u.pix"
   };

   PIXMAP Pix_eight_hun_lower[] =
   {
      #include "800l.pix"
   };

   PIXMAP Pix_nine_hun_upper[] =
   {
      #include "900u.pix"
   };

   PIXMAP Pix_nine_hun_lower[] =
   {
      #include "900l.pix"
   };

   PIXMAP Pix_thousand_upper[] =
   {
      #include "1000u.pix"
   };

   PIXMAP Pix_thousand_lower[] =
   {
      #include "1000l.pix"
   };

   PIXMAP Pix_fifteen_hun_upper[] =
   {
      #include "1500u.pix"
   };

   PIXMAP Pix_fifteen_hun_lower[] =
   {
      #include "1500l.pix"
   };

   PIXMAP Pix_free_upper[] =
   {
      #include "freeu.pix"
   };

   PIXMAP Pix_free_lower[] =
   {
      #include "freel.pix"
   };

   PIXMAP Pix_lose_turn_upper[] =
   {
      #include "loseu.pix"
   };

   PIXMAP Pix_lose_turn_lower[] =
   {
      #include "losel.pix"
   };

   PIXMAP Pix_broke_upper[] =
   {
      #include "brokeu.pix"
   };

   PIXMAP Pix_broke_lower[] =
   {
      #include "brokel.pix"
   };

   PIXMAP Arrow[] =
   {
      #include "arrow.pix"
   };
#endif


/*
** The wheel slots table contains pointers to each pixmap of a slot.
** The Pix_type array has three sections.  If it is only indexed
** in the first section then it is the beginner wheel.  If the second
** section is included then it is the novice wheel.  If the entire
** array is indexed into then it is the expert whee.
*/

PIXMAP *Pix_type[NUMBER_SLOTS_EXPERT << 1] = {
   Pix_four_hun_lower,
   Pix_four_hun_upper,
   Pix_sev_hun_lower,
   Pix_sev_hun_upper,
   Pix_three_hun_lower,
   Pix_three_hun_upper,
   Pix_fifteen_hun_lower,
   Pix_fifteen_hun_upper,
   Pix_broke_lower,
   Pix_broke_upper,
   Pix_thousand_lower,
   Pix_thousand_upper,
   Pix_eight_hun_lower,
   Pix_eight_hun_upper,
   Pix_free_lower,
   Pix_free_upper,
   Pix_two_hun_lower,
   Pix_two_hun_upper,
   Pix_five_hun_lower,
   Pix_five_hun_upper,
   Pix_nine_hun_lower,
   Pix_nine_hun_upper,
   Pix_six_hun_lower,
   Pix_six_hun_upper,
   Pix_one_hun_lower,
   Pix_one_hun_upper,
   Pix_sev_hun_lower,
   Pix_sev_hun_upper,
   Pix_lose_turn_lower,
   Pix_lose_turn_upper,
   Pix_thousand_lower,
   Pix_thousand_upper,
   
   Pix_nine_hun_lower,
   Pix_nine_hun_upper,
   Pix_fifteen_hun_lower,
   Pix_fifteen_hun_upper,
   Pix_four_hun_lower,
   Pix_four_hun_upper,
   Pix_sev_hun_lower,
   Pix_sev_hun_upper,
   Pix_lose_turn_lower,
   Pix_lose_turn_upper,
   Pix_five_hun_lower,
   Pix_five_hun_upper,

   Pix_fifteen_hun_lower,
   Pix_fifteen_hun_upper,
   Pix_lose_turn_lower,
   Pix_lose_turn_upper,
   Pix_eight_hun_lower,
   Pix_eight_hun_upper,
   Pix_thousand_lower,
   Pix_thousand_upper,
   Pix_broke_lower,
   Pix_broke_upper,
   Pix_six_hun_lower,
   Pix_six_hun_upper
};

/*
** The Wheel_values array has three sections.  If it is only indexed
** in the first section then it is the beginner wheel.  If the second
** section is included then it is the novice wheel.  If the entire
** array is indexed into then it is the expert whee.
*/
STATE_TYPE Wheel_values[NUMBER_SLOTS_EXPERT] = {
   FOUR,
   SEVEN,
   THREE,
   FIFTEEN,
   BROKE,
   TEN,
   EIGHT,
   FREE_SPIN,
   TWO,
   FIVE,
   NINE,
   SIX,
   ONE,
   SEVEN,
   LOSE_TURN,
   TEN,

   NINE,
   FIFTEEN,
   FOUR,
   SEVEN,
   LOSE_TURN,
   FIVE,

   FIFTEEN,
   LOSE_TURN,
   EIGHT,
   TEN,
   BROKE,
   SIX
};

