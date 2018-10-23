#pragma once

#include <string>

using std::string;

enum class State;

/*
../Users/userId/
userId: 오직 알파벳 소문자

소스 파일 이름: 제출번호.문제번호.년-월-일-시간-분-초.cpp
실행 파일: (소스 파일 이름).bin
제출한 코드의 stdout: (소스 파일 이름).stdout
제출한 코드의 컴파일러 에러 메시지 stderr: (소스 파일 이름).compile.stderr

FilePath: 그 파일의 Full 경로, 물론 파일이름 까지
Path: 오직 경로(마지막 '/' 붙임)
*/
struct Mark
{
public:
	bool support = true;
	string msg = "";

	const string userId;		// 사용자 id
	const string srcFileName;	// 채점 소스 파일 이름
	const int problemNum;		// 문제 번호

	const int judge_number;

	const string srcFilePath;	// 채점 소스 파일 경로
	const string binFilePath;	// 실행 파일 경로

	// 하나의 케이스 마다 각 in, out이 존재
//	const string proInputPath;	// 문제 입력 파일 경로
//	const string acDataPath;	// 문제 출력(정답) 파일 경로

	const string problemNumPath;	// 문제 번호 경로(../Problems/1000/)

	const string execute_stdout_file_path;	// 표준 출력 파일 경로
	const string compile_stderr_file_path;	// 표준 에러 출력 파일 경로

	State state;				// 현재 상태

	const char** compileCmd = nullptr;	// 검파일 명령어 상수 문자열 배열

	int TC_NUM;		// 테스트 케이스 개수
	int LIM_TIME_SEC;	// 제한 시간(ms)
	int LIM_MEM_MB;	// 제한 메모리(mb)

	int USE_TIME_MS;
	int USE_MEM_MB;

	int AC_NUM;		// 내가 맞은 테스트 케이스 수

public:
	Mark(const string& userId, const string& srcFileName, const int problemNum, const int judge_number = -1);
	~Mark();
};