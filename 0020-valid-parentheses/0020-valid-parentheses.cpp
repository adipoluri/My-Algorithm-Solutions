#include <stack>
class Solution {
public:
    bool isValid(string s) {
        std::vector<char> open;
        for (auto letter : s) {
            if( letter=='{' || letter=='[' || letter=='(') {
                open.push_back(letter);   
            } else {
                if(open.empty())
                    return false;
                
                switch(letter){
                    case '}':
                        if(open.back() != '{')
                           return false;
                        break;
                    case ']':
                        if(open.back() != '[')
                           return false;
                         break;
                    case ')': 
                        if(open.back() != '(')
                           return false;
                        break;
                }
                open.pop_back();
            }
        }
        return open.empty();
    }
};