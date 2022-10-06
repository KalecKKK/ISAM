#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "opDefines.h"
#define REG_NUM 16
using namespace std;

class Cmd {
public:
	int id, op, opi[3];
	string flag;

	Cmd() :id(), op(), opi(), flag() {}

	Cmd(int opc, const string& flagName, int index = 0)
		:id(index), op(opc), opi(), flag(flagName) {}

	Cmd(int opc, int op1 = 0, int op2 = 0, int op3 = 0, int index = 0) :
		id(index), op(opc), flag()
	{
		opi[0] = op1; opi[1] = op2; opi[2] = op3;
	}

	void output(ostream& os) {
		os  << hex << setfill('0') << setw(2) << opi[2]
			<< hex << setfill('0') << setw(2) << opi[1]
			<< hex << setfill('0') << setw(2) << opi[0]
			<< hex << setfill('0') << setw(2) << op;
	}
};

class Code {
private:
	map<string, int>	_names;
	map<int, int>		_const_num;
	map<string, int>	_marks;
	list<string>		_reg_alloc[REG_NUM];
	list<Cmd>			_cmds;

	int GetFreeReg();
	void GetOp(string cmdLine, int& l, int& r, int begin = 0);
	int CreateConstNum(int num);

	// 提取命令和参数，处理数字、所有寄存器代称和常数代称
	// 处理后仅剩Mark和Jmp标识符
	Cmd Decode(const string& cmdLine);
	void GenerateMapping();
public:
	Code(istream& ins);
	virtual ~Code() {}

	void ShowRegAlloc();
	void CheckRegOccupy();
	void Report();
	void output(ostream& os);
};

int main() {
	try {
		Code code(cin);
		code.output(cout);
		code.Report();
	}
	catch (const char* str) {
		cout << str;
	}
	catch (...) {
		cout << "Unkown Error!";
		throw;
	}
	return 0;
}

Code::Code(istream& ins) {
	for (int i = 0; i < REG_NUM; ++i)
		_names.emplace("r" + to_string(i), i);
	string line;
	while (getline(ins, line)) {
		bool isValid = false;
		for (auto& c : line)
			if (!isspace(c)) {
				isValid = true;
				break;
			}
		if (!isValid) continue;
		Cmd cmd = Decode(line);
		if (cmd.op < CMD_ID_BOUNDARY
			|| cmd.op == CMD_MARK_ID)
			_cmds.push_back(cmd);
	}
	_cmds.push_front(Cmd());
	GenerateMapping();
}

int Code::GetFreeReg() {
	for (int i = 0; i < REG_NUM; ++i)
		if (_reg_alloc[i].empty())
			return i;
	throw "No free space for new!";
}

void Code::GetOp(string cmdLine, int& l, int& r, int begin) {
	l = begin;
	while (l < cmdLine.size()
		&& (isspace(cmdLine[l]) || cmdLine[l] == ',')) ++l;
	r = l;
	while (r < cmdLine.size()
		&& !(isspace(cmdLine[r]) || cmdLine[r] == ',')) ++r;
}

int Code::CreateConstNum(int num) {
	int r = GetFreeReg();
	_const_num[num] = r;
	_cmds.push_front(Cmd(CMD_WH_ID, r, (num & 0x00FF0000) >> 16, (num & 0xFF000000) >> 24));
	_cmds.push_front(Cmd(CMD_WL_ID, r,  num & 0x000000FF       , (num & 0x0000FF00) >> 8 ));
	return r;
}

Cmd Code::Decode(const string& cmdLine) {
	Cmd cmd;
	int opl, opr;
	GetOp(cmdLine, opl, opr);
	cmd.op = GetOpIndex(cmdLine.substr(opl, opr - opl));
	if (cmd.op == -1)
		throw "unknown operator!";
	for (int opIndex = 0; opIndex < 3;) {
		GetOp(cmdLine, opl, opr, opr);
		if (opl == opr) break;
		string opistr = cmdLine.substr(opl, opr - opl);
		if (isdigit(opistr[0]) || opistr[0] == '-') {
			// get the digit
			int num = atoi(opistr.c_str());
			// operator parameter digit
			if (cmd.op == CMD_JMP_ID) {
				cmd.opi[0] = num & 0x00FF;
				break;
			}
			else if (cmd.op == CMD_WH_ID ||
					 cmd.op == CMD_WL_ID)
			{
				cmd.opi[1] =  num & 0x00FF;
				cmd.opi[2] = (num & 0xFF00) >> 8;
				break;
			}
			else { // const digit
				if (!_const_num.count(num)) {
					// new register digit
					// add this digit to unused register
					int r = CreateConstNum(num);
					_reg_alloc[r].push_back("_const_" + to_string(num));
				}
				cmd.opi[opIndex++] = _const_num[num];
			}
		}
		else { // string identifier
			if (cmd.op == CMD_JMP_ID || cmd.op == CMD_MARK_ID) {
				if (_names.count(opistr))
					throw "Duplicate identifier!";
				cmd.flag = opistr;
				break;
			}
			else { // normal condition
				if (_names.count(opistr)) {
					switch (cmd.op) {
					case CMD_NEW_ID:
						throw "Duplicate identifier!";
					case CMD_NAME_ID:
						if (opIndex != 0)
							throw "Duplicate identifier!";
						// else没有break，需要执行default
					default: 
						cmd.opi[opIndex++] = _names[opistr];
					}
				}
				else {
					// deal Basic Instruction Set
					if (cmd.op < CMD_ID_BOUNDARY)
						throw "undefined identifier!";
					else { // deal Extended Instruction Set
						int r = -1;
						switch (cmd.op) {
						case CMD_NEW_ID:
							r = GetFreeReg();
							_names[opistr] = r;
							_reg_alloc[r].push_back(opistr);
							_cmds.push_front(Cmd(CMD_RST_ID, r));
							break;
						case CMD_NAME_ID:
							if (opIndex == 0)
								throw "undefined identifier!";
							r = cmd.opi[0];
							_names[opistr] = r;
							_reg_alloc[r].push_back(opistr);
							break;
						default: throw "unknown operator!";
						}
					}
				}
			}
		}
	}
	return cmd;
}

void Code::GenerateMapping() {
	list<Cmd> res;
	int index = 0;
	// 第一次循环给Mark的Flag标号，res保留基本指令集
	for (auto& cmd : _cmds) {
		if (cmd.op == CMD_MARK_ID) {
			if (cmd.flag.empty())
				throw "incomplete Mark instructions!";
			_marks[cmd.flag] = index;
		}
		else {
			cmd.id = index++;
			res.push_back(cmd);
		}
	}
	// 第二次循环定义Jmp指向
	for (auto& cmd : res) {
		if (cmd.op == CMD_JMP_ID) {
			if (!_marks.count(cmd.flag))
				throw "jump to unkown place!";
			cmd.opi[0] = _marks[cmd.flag];
		}
	}
	// 完整的编译机器码交回_cmds成员
	_cmds = move(res);
}

void Code::ShowRegAlloc() {
	cout << "\nFrom generate : register usage\n";
	for(int i = 0;i < REG_NUM;++i)
		if (!_reg_alloc[i].empty()) {
			cout << "Reg No." << i << ':';
			for (auto& name : _reg_alloc[i])
				cout << ' ' << name;
			cout << endl;
		}
}

void Code::CheckRegOccupy() {
	cout << "\nFrom generate : Checking register usage...\n";
	for (int i = 0; i < REG_NUM; ++i)
		if (_reg_alloc[i].size() > 1) {
			bool andFlag = false;
			cout << "Warning: ";
			for (auto& name : _reg_alloc[i]) {
				if (andFlag) cout << " & ";
				else andFlag = true;
				cout << '\'' << name << '\'';
			}
			cout << " share the same register r" << i << "!\n";
		}
}

void Code::Report() {
	ShowRegAlloc();
	CheckRegOccupy();
}

void Code::output(ostream& os) {
	/*
	for (auto& cmd : _cmds) {
		cmd.output(os);
		os << endl;
	}
	/*/
	int index = 0;
	for (auto& cmd : _cmds) {
		os << "cmd_mem[" << dec << index++ << "] = 32\'h";
		cmd.output(os);
		os << ';' << endl;
	}
	//*/
}
