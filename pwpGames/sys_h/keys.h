#define EXIT_KEY				0x1b
#define BACKSPACE_KEY		0x08
#define TAB_KEY				0x09
#define SPACE_KEY    	   0x20
#define RETURN_KEY			0x0d
#define CURSRIGHT_KEY		0x1c
#define CURSLEFT_KEY			0x1d
#define CURSUP_KEY			0x1e
#define CURSDOWN_KEY			0x1f
#define CODE_KEY	 			0x8700
#define SELECT_KEY			0x8800
#define SELECT_ADV_KEY 	   0x8900
#define WPTYPE_KEY      	0x8a
#define SCRN_UP_KEY	    	0x8b
#define SCRN_DOWN_KEY	   0x8c
#define ADVANCE_KEY 			0x8d
#define PAPER_IN_KEY	   	0x8e
#define DELETE_KEY			0x8f
#define WORDERASE_KEY	 	0x90
#define HELP_KEY        	0x91
#define SPELL_KEY	      	0x92
#define UNDO_KEY  	      0x93
#define DICT_KEY     	   0x94
#define FIND_KEY	         0x95
#define GOTO_KEY   	      0x96
#define LIST_KEY      	   0x97
#define PRIOR_COL_KEY		0x98
#define NEXT_COL_KEY			0x99

#define LOCK_KEY				0x9d
#define SELECT_OUT_KEY 		0x9e

#define LID_OPEN				0x9f
#define LID_CLOSE				0xa0

#define CODE_ADV_KEY			0x878d
#define MARK_RIGHT_KEY	 	0x881c
#define MARK_LEFT_KEY 		0x881d
#define MARK_UP_KEY   		0x881e
#define MARK_DOWN_KEY		0x881f
#define INDENT_ON_KEY	   0x8734
#define INDENT_OFF_KEY  	0x8735
#define CENTER_KEY 	      0x8736
#define AUTO_RETURN_KEY 	0x8737
#define PAGE_END_KEY		   0x8739
#define UNDERLINE_KEY   	0x872d
#define STOP_KEY				0x8773
#define COPY_KEY  	      0x8763
#define VOL_KEY				0x8776
#define BOLD_KEY				0x8762
#define NEXT_KEY				0x876e
#define MOVE_KEY				0x876d
#define LINE_ERASE_KEY	   0x8790
#define WORDRIGHT_KEY   	0x871c
#define WORDLEFT_KEY 	   0x871d
#define KBII_KEY				0x876b
#define SUPER_KEY				0x876f
#define SUB_KEY				0x8770
#define UP_ARROW_KEY			CURSUP_KEY
#define DOWN_ARROW_KEY		CURSDOWN_KEY
#define RIGHT_ARROW_KEY		CURSRIGHT_KEY
#define LEFT_ARROW_KEY		CURSLEFT_KEY


enum lock_stat {
	CAPSLOCK_ON, CAPSLOCK_OFF,
	NUMLOCK_ON, NUMLOCK_OFF
};

#define CODE_RET 				0x870d
#define CODE_B          	0x8762
#define CODE_F          	0x8766
#define CODE_G          	0x8767
#define CODE_N   				0x876e
#define CODE_O   				0x876f
#define CODE_P   				0x8770
#define CODE_Q             0x8771
#define CODE_Z             0x877a			/* Address Merge */
#define CODE_DEL 				0x878f

#define n_KEY					0x6e				/* Lower Case */
#define o_KEY					0x6f				/* Lower Case */
#define x_KEY					0x78				/* Lower Case */
#define y_KEY					0x79				/* Lower Case */

#define CODE_D					0x8764

#define HALF_KEY				0xf8
#define THIRD_KEY				0x80
#define QUARTER_KEY			0xf7
#define THREE_QTR_KEY		0xf5
#define CENT_KEY				0xbf
#define UK_POUND_KEY			0xbb
#define UMLAUT_KEY			0xab
#define DEGREE_KEY			0xb3
#define LC_NTILDE_KEY		0xb7
#define UC_NTILDE_KEY		0xb6
#define UD_QUEST_KEY			0xb9
#define UD_BANG_KEY			0xb8
