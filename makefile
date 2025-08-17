# Makefile - echo 프로그램 빌드 파일 (리눅스 전용)

# 컴파일러 설정
CXX = g++
CXXFLAGS = -std=c++11 -Wall
LDFLAGS = -lpthread  # 스레드 라이브러리 링크

# 타겟 파일들
TARGETS = echo-server echo-client

# 기본 타겟: 모든 프로그램 빌드
all: $(TARGETS)

# echo-server 빌드 규칙
echo-server: echo-server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# echo-client 빌드 규칙  
echo-client: echo-client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

# 생성된 파일 삭제
clean:
	rm -f $(TARGETS)

.PHONY: all clean