#pragma once
#include <string>

// Basic Instruction Set
#define CMD_PASS_STR	"pass"
#define CMD_JMP_STR		"jmp"
#define CMD_WH_STR		"wh"
#define CMD_WL_STR		"wl"
#define CMD_MOV_STR		"mov"
#define CMD_RST_STR		"rst"
#define CMD_IFEQ_STR	"ifeq"
#define CMD_IFNQ_STR	"ifnq"
#define CMD_IFGQ_STR	"ifgq"
#define CMD_IFLQ_STR	"iflq"
#define CMD_ADD_STR		"add"
#define CMD_SUB_STR		"sub"
#define CMD_NOT_STR		"not"
#define CMD_OR_STR		"or"
#define CMD_AND_STR		"and"
#define CMD_XOR_STR		"xor"
#define CMD_SL_STR		"sl"
#define CMD_SR_STR		"sr"
#define CMD_OUT_STR		"out"

#define CMD_PASS_ID		0x00
#define CMD_JMP_ID		0x01
#define CMD_WH_ID		0x02
#define CMD_WL_ID		0x03
#define CMD_MOV_ID		0x04
#define CMD_RST_ID		0x05
#define CMD_IFEQ_ID		0x06
#define CMD_IFNQ_ID		0x07
#define CMD_IFGQ_ID		0x08
#define CMD_IFLQ_ID		0x09
#define CMD_ADD_ID		0x0A
#define CMD_SUB_ID		0x0B
#define CMD_NOT_ID		0x0C
#define CMD_OR_ID		0x0D
#define CMD_AND_ID		0x0E
#define CMD_XOR_ID		0x0F
#define CMD_SL_ID		0x10
#define CMD_SR_ID		0x11
#define CMD_OUT_ID		0x12

#define CMD_ID_BOUNDARY	0xF0

// Extended Instruction Set
#define CMD_NEW_STR		"new"
#define CMD_NAME_STR	"name"
#define CMD_MARK_STR	"mark"

#define CMD_NEW_ID		0xF0
#define CMD_NAME_ID		0xF1
#define CMD_MARK_ID		0xF2

int GetOpIndex(const std::string& str);
