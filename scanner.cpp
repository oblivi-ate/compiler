#include "Scanner.h"
#include "dfa.h"
#include <iostream>
#include <fstream>
#include <sstream>

class Scanner {
public:
    Scanner(std::istream& in) : in(in) {}

    void scan(Token_List* token_list);

private:
    std::istream& in; // 输入流
    char currentChar;  // 当前字符
    int position = 0; // 当前位置
};

void Scanner::scan(Token_List* token_list) {
    initdfa(); // 初始化dfa状态
    currentChar = in.get(); // 读取第一个字符

    while (currentstate != DONE && currentstate != ERROR && !in.eof()) {
        switch (currentstate) {
            case START:
                // 根据当前字符进行状态转移
                break;
            case READ_EQ:
                // 处理等号(...)
            case READ_LT:
                // 处理小于号(...)
            case READ_GT:
                // 处理大于号(...)
            case READ_NOT:
                // 处理感叹号(...)
            case IN_NUM:
                // 处理数字(...)
            case IN_ID:
                // 处理标识符(...)
            case INCOMMENT:
                // 处理注释(...)
            case WAIT_OVER:
                // 等待注释结束(...)
            default:
                // 其他状态处理
                break;
        }
        // 读取下一个字符
        currentChar = in.get();
        position++;
    }

    if (currentstate == ERROR) {
        std::cerr << "Error occurred during scanning." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open file: " << argv[1] << std::endl;
        return 1;
    }

    Token_List token_list; // 用于存储token的链表
    Scanner scanner(inputFile);
    scanner.scan(&token_list);

    // 打印token列表（示例代码，需要根据实际情况实现Token_List的打印功能）
    Token_List* current = &token_list;
    while (current != nullptr) {
        std::cout << current->token << std::endl;
        current = current->next;
    }

    inputFile.close();
    return 0;
}