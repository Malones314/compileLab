/**实验要求
    *1、对给定的程序通过词法分析器能够识别一个个单词符号，通过对给定路径的文件的分析后，以二元式（单词种别码，单词符号的属性值）显示；
    *2、将单词分为如下的五类：
    *    1）关键字：begin, end, if, then, else, while...do
    *    2）运算符： +，-，*，/，:= ，>，<
    *    3）界符：(，)，{ ，} ，[ ，] ，；
    *    4）普通标识符
    *    5）无符号实数
    ***/
/**  Author:                  翟睿辰
    * Data:                      2022/03/30
    * Environment:         VisualStudio 2019
    * langue:                  Cpp  
    ***/
# include<iostream>
# include<string>
# include<fstream>
# include<sstream>
# include<vector>
# include<map>
using namespace std;

bool isNumber ( string& s );                 //数字
bool isOperator ( string& s );                //运算符
bool isIdentifier ( string& s );                //标识符
bool isKeywords ( string& s );               //关键字
bool isOperator ( char c );                  //运算符
bool isDelimiters ( string& s );              //界符
bool isDelimiters ( char c );                //界符
string result ( string& s );                      //根据传入的参数s产生对应的输出
int main ( ) {

    //文件位置
    string file = ( "C:\\Users\\Malones\\Desktop\\test.txt" );
    ifstream input ( file );
    //输出文件位置
    ofstream output ( "C:\\Users\\Malones\\Desktop\\result.txt", ofstream::app );

    string copy;

    getline ( input, copy, '\0' );
    cout << copy << endl;//测试是否正确

    //此时input已经指到了文件尾，为了后面的读取，需要关闭再打开
    input.close ( );
    input.open ( file );

    //要求以原数据与结果对照的形式输出并保存在文件中，并把结果输出到屏幕。
    
    output << "原数据:\n";
    output << copy;
    output << "\n处理后结果:\n";

    string str;
    string words;

    cout << "处理后结果:\n";
    while ( getline ( input, str ) ) { //读取文件每一次读取一行,遇到EOF结束 
        //从输入流中获取单词，需要用到输入流对象，即istringstream
        istringstream strCin ( str );
        string s;
        while ( strCin >> words ) { 

            //测试
            //cout << "......................................" << words << endl;
            
            //1.关键字肯定是单独作为一个string
            if ( isKeywords ( words ) ) {
                s = result ( words );
                cout << s << endl;
                output << s << endl;
                continue;
            }

            //2,对string进行扫描，肯定是标识符，运算符，逗号分号,数字等等混合在一起的单词
            if ( words.length ( ) == 1 ) {      //长度为1，只能是单个数字，标识符，界符，运算符
                if ( isNumber ( words ) || isDelimiters( words ) || isIdentifier( words ) || isOperator( words) ) {
                    s = result ( words );
                    cout << s << endl;
                    output << s << endl;
                    continue;
                }
            } else if ( words.length ( ) == 2 && words [0] == ':' && words [1] == '=' ) {
                s = result ( words );
                cout << s << endl;
                output << s << endl;
                continue;
            } else {                    //长度大于等于2且非‘:=’运算符
                if ( isIdentifier ( words ) ) {         //看是否是标识符
                    s = result ( words );
                    cout << s << endl;
                    output << s << endl;
                    continue;
                } else {                                        //非标识符，需要分割，或者错误
                    vector<string> wordsStr;
                    for ( int i = 0; i < words.length ( ); ) {
                        if ( words [i] <= '9' && words [i] >= '0' || words [i] <= 'z' && words [i] >= 'a' || words [i] >= 'A' && words [i] <= 'Z' ) {
                            int j = 0;                        //记录数字或字母或下划线或.到哪结束
                                                                 //因为在标识符和数字判断部分分别判断了字符串是否符合标识符或者数字的标准，
                                                                //所以在此处无需判断是否符合标准（标识符中不能出现'.'，数字中不能出现超过1个'.'）
                            while ( words[i+j] == '.' || words[j+i] == '_' || words [j + i] <= '9' && words [j + i] >= '0' || words [j + i] <= 'z' && words [j + i] >= 'a' || words [j + i] >= 'A' && words [j + i] <= 'Z' )
                                j++;
                            wordsStr.push_back ( words.substr ( i, j ) );
                            i += j;
                        } else if ( words [i] == ':' ) {
                            if ( words [i + 1] == '=' ) {
                                wordsStr.push_back ( words.substr ( i, 2 ) );
                                i += 2;
                            } else {
                                wordsStr.push_back ( ":" );
                                i++;
                            }
                        }
                        /*
                        else if ( isOperator ( words [i] ) || isDelimiters ( words [i] ) ) {        //是运算符、界符
                            wordsStr.push_back ( to_string ( words [i] ) );
                            i++;
                        } 
                        */
                        //单字符运算符、界符都可以直接单个字符push到wordsStr中
                        else {
                            string tmp ( 1, words [i] );
                            wordsStr.push_back ( tmp );
                            i++;
                        }
                    }
                    string ss;
                    for ( int i = 0; i < wordsStr.size ( ); i++ ) {
                        ss = result ( wordsStr[i] );
                        cout << ss << endl;
                        output << ss << endl;
                    }

                }
            }
        }
    }
    output << endl;
    output.close ( );
    input.close ( );
    return 0;
}

bool isIdentifier ( string& s ) {            //标识符

    if ( !isKeywords ( s ) ) {                 //标识符不能是关键字 
        if ( ( s [0] >= 'a' && s [0] <= 'z' ) || ( s [0] >= 'A' && s [0] <= 'Z' ) ) {       //字母开头
            for ( int i = 1; i < s.length ( ); i++ ) {                                                      //后面是字母、数字、下划线
                if ( s[i] == '_' || ( s [i] >= 'a' && s [i] <= 'z' ) || ( s [i] >= 'A' && s [i] <= 'Z' )
                    || ( s [i] >= '0' && s [i] <= '9' ) )
                    continue;
                else return false;
            }
            return true;
        }
        return false;           //非字母开头，直接error
    }

    return false;              //标识符是关键字，直接error
}
bool isKeywords ( string& s ) {           //关键字

    static vector<string> keyWord = { "if", "then", "else", "begin", "end", "while", "do" };
    auto result = find ( keyWord.begin ( ), keyWord.end ( ), s );
    if ( result != keyWord.end ( ) )
        return true;

    else 
        return false;
}
bool isNumber ( string& s ) {          //整型数字

    bool one = false;
    if ( s [0] >= '0' && s [0] <= '9' )     {
        for ( int i = 1; i < s.length ( ); ++i ) {
            if ( s [i] >= '0' && s [i] <= '9' ) {
                continue;
            } else if ( s [i] == '.' ) {
                if ( one == false )
                    one == true;
                else
                    return false;
            } else return false;
        }
        return true;
    }
    return false;
}

bool isDelimiters ( string& s ) {         //界符
    static vector< string> delimitersVec = { "(", ")", "{", "}", "[", "]", ";" };
    auto result = find ( delimitersVec.begin ( ), delimitersVec.end ( ), s );
    if ( result != delimitersVec.end ( ) )
        return true;
    return false;
}
bool isDelimiters ( char c ) {           //界符
    static vector< char> delimitersVec = { '(', ')', '{', '}', '[', ']', ';' };
    auto result = find ( delimitersVec.begin ( ), delimitersVec.end ( ), c );
    if ( result != delimitersVec.end ( ) )
        return true;
    return false;
}

bool isOperator ( string& s ) {           //运算符

    static vector<string> opeVec = { "+", "-", "*", "/", "<", ":=", ">" };
    vector<string>::iterator result = find ( opeVec.begin ( ), opeVec.end ( ), s );
    if ( result != opeVec.end ( ) )
        return true;
    return false;

}

bool isOperator ( char c ) {            //运算符

    static vector<char> opeVec = { '+', '-', '*', '/', '<', '>' /* ":=" */ };
    vector<char>::iterator result = find ( opeVec.begin ( ), opeVec.end ( ), c );
    if ( result != opeVec.end ( ) )
        return true;
    return false;
}

string result ( string& s ) {                 //根据传入的参数s产生对应的输出
    //标识符
    if ( isIdentifier ( s ) )
        return s + "\t$标识符";

    //关键字
    if ( isKeywords ( s ) )
        return   s + "\t$关键字";

    //整型常量
    if ( isNumber ( s ) )
        return  s + "\t$无符号实数";

    //运算符
    if ( isOperator ( s ) )
        return s+"\t$运算符";

    //界符
    if ( isDelimiters ( s ) )
        return s + "\t$界符";

    return "Error";
}
