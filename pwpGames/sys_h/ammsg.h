		/* Address Merge PSOS System Messages. */


enum am_commands { 

		L_AMCREMB = 1300,
		LIST_CMD, DOC_CMD, PRINT_CMD, DISK_CMD, L_EXIT,
		L_EMALST, D_AMDSKMB, L_INDEMB, EXIT_AM_CMD, MERGE_CMD,
		SORT_CMD, L_SRTLSTM, P_AMPRNMB, M_AMERGEM, AM_DOWNLOAD_EXIT, 
		M_COND, D_DISPMB, P_AMPLABM, D_AMDISPMB, D_FN_WINDOW, D_AMDSKDM,
		AM_EXIT, M_AMINSPAPER, M_SELMRGM, M_CONMRG, P_AMDLMB, P_AMPLM,
		D_DSKGEN,

		/* don't add at end any more unless new application */

		LAST_AM_CMD };	

enum am_commands am_command;

