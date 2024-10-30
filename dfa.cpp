#include "dfa.h"
#include "token.h"
#include <unordered_map>
#include <utility>
#include <string>

STATE currentstate = START;
int currentpos = 0;
std::string info = "";

std::unordered_map<Token, std::string> keywords = {
    {Token(IF, "if"), "if"},
    {Token(ELSE, "else"), "else"},
    {Token(WHILE, "while"), "while"},
    {Token(RETURN, "return"), "return"},
    {Token(INT, "int"), "int"},
    {Token(VOID, "void"), "void"},
    {Token(PLUS, "+"), "+"},
    {Token(MINUS, "-"), "-"},
    {Token(TIMES, "*"), "*"},
    {Token(DIVIDE, "/"), "/"},
    {Token(LT, "<"), "<"},
    {Token(LTE, "<="), "<="},
    {Token(GT, ">"), ">"},
    {Token(GTE, ">="), ">="},
    {Token(EQ, "=="), "=="},
    {Token(NEQ, "!="), "!="},
    {Token(ASSIGN, "="), "="},
    {Token(SEMICOLON, ";"), ";"},
    {Token(COMMA, ","), ","},
    {Token(LPAREN, "("), "("},
    {Token(RPAREN, ")"), ")"},
    {Token(LBRACKET, "["), "["},
    {Token(RBRACKET, "]"), "]"},
    {Token(LBRACE, "{"), "{"},
    {Token(RBRACE, "}"), "}"},
    {Token(ID, "id"), "id"},
    {Token(NUMBER, "number"), "number"},
    {Token(LETTER, "letter"), "letter"},
    {Token(WS, "ws"), "ws"},
    {Token(INDENT, "indent"), "indent"},
    {Token(DEDENT, "dedent"), "dedent"},
    {Token(ENTER, "enter"), "enter"},
    {Token(ENDFILE, "endfile"), "endfile"},

    // ...  
};


void addToken(Token_List *token_list, Token token)
{
    Token_List *new_token = new Token_List(token);
    token_list->next = new_token;
    new_token->prev = token_list;
    token_list = new_token;
}


void TransitionStart(char c, Token_List *tk, bool is_EOF){
    char info = "";
    if(is_EOF){
        currentstate = DONE;
        currentpos++;
        addToken(tk, Token(ENDFILE, ""));
        return;
    }
    if((currentstate==START||currentstate==NOT_DONE||currentstate==DONE)&&is_EOF){
    switch (c)
    {
    case '+' :
        currentstate = DONE;
        currentpos++;
        addToken(tk, Token(PLUS, "PLUS"));
        break;
    case'-':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(MINUS, "MINUS"));
        break;
    case'*':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(TIMES, "TIMES"));
        break;
    case'%':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(MOD, "MOD"));
        break;
    case';':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(SEMICOLON, "SEMICOLON"));
        break;
    case',':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(COMMA, "COMMA"));
        break;
    case'(':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(LPAREN, "LPAREN"));
        break;
    case')':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(RPAREN, "RPAREN"));
        break;
    case'[':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(LBRACKET, "LBRACKET"));
        break;
    case']':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(RBRACKET, "RBRACKET"));
        break;
    case'{':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(LBRACE, "LBRACE"));
        break;
    case'}':
    	currentstate = DONE;
        currentpos++;
        addToken(tk, Token(RBRACE, "RBRACE"));
        break;
    case '=':
        currentstate = READ_EQ;
        currentpos++;
        break;
    case '>':
        currentstate = READ_GT;
        currentpos++;
        break;
    case '!':
        currentstate = READ_NOT;
        currentpos++;
        break;
    case '<':
        currentstate = READ_LT;
        currentpos++;
        break;
    case '/':
    	currentstate=READ_OVER;
    	currentpos++;
    case'#':
    	currentstate=COMMENTING;
    	currentpos++;
        //
    case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
        currentstate = IN_NUM;
        currentpos++;
        info += c;
        break;
    case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' :
    	currentstate = IN_LET;
        currentpos++;
        info += c;
        break;
    default:  //�����������ַ�ʱֱ�ӱ���return����main����ѭ���������һ���ж�token�����Ƿ�Ϊerror����䣬��������˳�ѭ�� 
        currentstate=ERROR;
        currentpos++;
        addToken(tk, Token(ERROR, "ERROR! Illegal character!"));
        return;
    }
}
	 if(currentstate==READ_EQ&&is_EOF){ //���Ѿ�����һ����=��ʱ 
	 	switch (c)
    {
    case '=':  //�ڶ��� ��=�� ���룬 ״̬ΪDONE����tokenlist�м�����EQΪTokentype���½ڵ� 
        currentstate = DONE;
        addToken(tk, Token(EQ, ""));
        currentpos++;
        break;
    default://û�н��յ��ڶ�����=��״̬ΪNOT_DONE 
        currentstate=NOT_DONE;
        addToken(tk, Token(ASSIGN, "ASSIGN")); //��֮ǰ¼��ĵ������Ŵ�������
        break;
    }
    if(currentstate==READ_GT&&is_EOF){
    	switch(c){
    		case'=':
    			currentstate = DONE;
        		addToken(tk, Token(GTE, "GREATER THAN OR EQUAL TO"));
        		currentpos++;
        		break;
        	default:
        		currentstate=NOT_DONE;
        		addToken(tk, Token(GT, "GREATER")); 
        		break;
		} 
	}
	if(currentstate==READ_LT&&is_EOF){
    	switch(c){
    		case'=':
    			currentstate = DONE;
        		addToken(tk, Token(LTE, "LESS THAN OR EQUAL TO"));
        		currentpos++;
        		break;
        	default:
        		currentstate=NOT_DONE;
        		addToken(tk, Token(LT, "LESS THAN")); 
        		break;
		} 
	}
	if(currentstate==READ_NOT&&is_EOF){
    	switch(c){
    		case'=':
    			currentstate = DONE;
        		addToken(tk, Token(NEQ, "NOT EQULA TO"));
        		currentpos++;
        		break;
        	default: //����ȡ��̾�ţ�֮��û�еȺ���ֱ�ӱ�������ֱ�ӷ��� �� 
        		currentstate=ERROR;
        		addToken(tk, Token(ERROR, "ERROR! There are only single ! appeared."));
        		return;
		} 
	}
	if(currentstate==IN_NUM&&is_EOF){ //ǰһ�������ֵ���� 
    	switch(c){
    		case '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9':
    			currentstate = IN_NUM;
        		currentpos++;
        		info+=c;
        		break;
        	default:  
        		currentstate=NOT_DONE;
        		addToken(tk, Token(NUMBER, info));
        		info="";
        		break;
		} 
	}
	if(currentstate==IN_LET&&is_EOF){ //ǰһ������ĸ����� 
    	switch(c){
    		case 'A' | 'B' | 'C' | 'D' | 'E' | 'F' | 'G' | 'H' | 'I' | 'J' | 'K' | 'L' | 'M' | 'N' | 'O' | 'P' | 'Q' | 'R' | 'S' | 'T' | 'U' | 'V' | 'W' | 'X' | 'Y' | 'Z' | 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' | 'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' | 'y' | 'z' :
    			currentstate = IN_LET;
        		currentpos++;
        		info+=c;
        		break;
        	default:  
        		currentstate=NOT_DONE;
        		switch(info){ //���֮ǰ�����Ӣ�ĵ����Ƿ�Ϊ�ؼ��ʣ������򷵻ز�ͬ���� 
        			case'if':
						addToken(tk, Token(IF, "IF"));
						break;
					case'else':
						addToken(tk, Token(ELSE, "ELSE"));
						break;
        			case'while':
						addToken(tk, Token(WHILE, "WHILE"));
						break;
					case'return':
						addToken(tk, Token(RETURN, "RETURN"));
						break;
					case'int':
						addToken(tk, Token(INT, "INT"));
						break;
					case'void':
						addToken(tk, Token(VOID, "VOID"));
						break;
					default:
						addToken(tk, Token(LETTER, info));
        				break;
				} 
				info="";
        		break;
		} 
	}
	if(currentstate==READ_OVER&&is_EOF){ //ǰһ����б�ܵ���� 
    	switch(c){
    		case '/' �� 
    			currentstate = COMMENTING;//COMMENTING��#���ۺ�//���۵���� 
        		currentpos++;
        		break;
        	case'*'��
				currentstate = INCOMMENT;//INCOMMENT��/*���۵���� 
        		currentpos++;
        		break;
        	default:  
        		currentstate=DONE;
        		addToken(tk, Token(DIVIDE, "DIVIDE"));
        		break;
		} 
	}
	if(currentstate==COMMENTING&&is_EOF){ 
    	switch(c){
    		case '\n' �� 
    			currentstate = DONE;
    			addToken(tk, Token(COMMENT, info));
    			info="";
        		currentpos++;
        		break;
        	default:  
        		info+=c;
        		currentpos++;
        		break;
		} 
	}
		if(currentstate==INCOMMENT&&is_EOF){ 
    	switch(c){
    		case '*' �� 
    			currentstate = WAIT_OVER;
        		currentpos++;
        		break;
        	default:  
        		info+=c;
        		currentpos++;
        		break;
		} 
	}
	if(currentstate==WAIT_OVER&&is_EOF){ 
    	switch(c){
    		case '*' �� 
        		currentpos++;
        		info+="*";
        		break;
        	case'/':
        		currentstate=DONE;
        		currentpos++;
        		addToken(tk, Token(COMMENT, info));
        		info="";
        	default:  
        		currentstate=INCOMMENT;
        		info+="*";
        		info+=c;
        		currentpos++;
        		break;
		} 
	}
	return;
}


void initdfa()
{
    // initialize the dfa
    currentstate = START;
    currentpos = 0;
}
