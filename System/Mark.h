#pragma once

#include <string>

using std::string;

enum class State;

/*
../Users/userId/
userId: ���� ���ĺ� �ҹ���

�ҽ� ���� �̸�: �����ȣ.������ȣ.��-��-��-�ð�-��-��.cpp
���� ����: (�ҽ� ���� �̸�).bin
������ �ڵ��� stdout: (�ҽ� ���� �̸�).stdout
������ �ڵ��� �����Ϸ� ���� �޽��� stderr: (�ҽ� ���� �̸�).compile.stderr

FilePath: �� ������ Full ���, ���� �����̸� ����
Path: ���� ���(������ '/' ����)
*/
struct Mark
{
public:
	bool support = true;
	string msg = "";

	const string userId;		// ����� id
	const string srcFileName;	// ä�� �ҽ� ���� �̸�
	const int problemNum;		// ���� ��ȣ

	const int judge_number;

	const string srcFilePath;	// ä�� �ҽ� ���� ���
	const string binFilePath;	// ���� ���� ���

	// �ϳ��� ���̽� ���� �� in, out�� ����
//	const string proInputPath;	// ���� �Է� ���� ���
//	const string acDataPath;	// ���� ���(����) ���� ���

	const string problemNumPath;	// ���� ��ȣ ���(../Problems/1000/)

	const string execute_stdout_file_path;	// ǥ�� ��� ���� ���
	const string compile_stderr_file_path;	// ǥ�� ���� ��� ���� ���

	State state;				// ���� ����

	const char** compileCmd = nullptr;	// ������ ��ɾ� ��� ���ڿ� �迭

	int TC_NUM;		// �׽�Ʈ ���̽� ����
	int LIM_TIME_SEC;	// ���� �ð�(ms)
	int LIM_MEM_MB;	// ���� �޸�(mb)

	int USE_TIME_MS;
	int USE_MEM_MB;

	int AC_NUM;		// ���� ���� �׽�Ʈ ���̽� ��

public:
	Mark(const string& userId, const string& srcFileName, const int problemNum, const int judge_number = -1);
	~Mark();
};