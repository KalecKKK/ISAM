#include "opDefines.h"

int GetOpIndex(const std::string& str) {
	if (str == CMD_PASS_STR)	return CMD_PASS_ID;
	if (str == CMD_JMP_STR)		return CMD_JMP_ID;
	if (str == CMD_WH_STR)		return CMD_WH_ID;
	if (str == CMD_WL_STR)		return CMD_WL_ID;
	if (str == CMD_MOV_STR)		return CMD_MOV_ID;
	if (str == CMD_RST_STR)		return CMD_RST_ID;
	if (str == CMD_IFEQ_STR)	return CMD_IFEQ_ID;
	if (str == CMD_IFNQ_STR)	return CMD_IFNQ_ID;
	if (str == CMD_IFGQ_STR)	return CMD_IFGQ_ID;
	if (str == CMD_IFLQ_STR)	return CMD_IFLQ_ID;
	if (str == CMD_ADD_STR)		return CMD_ADD_ID;
	if (str == CMD_SUB_STR)		return CMD_SUB_ID;
	if (str == CMD_NOT_STR)		return CMD_NOT_ID;
	if (str == CMD_OR_STR)		return CMD_OR_ID;
	if (str == CMD_AND_STR)		return CMD_AND_ID;
	if (str == CMD_XOR_STR)		return CMD_XOR_ID;
	if (str == CMD_SL_STR)		return CMD_SL_ID;
	if (str == CMD_SR_STR)		return CMD_SR_ID;
	if (str == CMD_OUT_STR)		return CMD_OUT_ID;

	if (str == CMD_NEW_STR)		return CMD_NEW_ID;
	if (str == CMD_NAME_STR)	return CMD_NAME_ID;
	if (str == CMD_MARK_STR)	return CMD_MARK_ID;
	return -1;
}
