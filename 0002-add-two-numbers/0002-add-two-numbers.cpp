/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *temp = new ListNode();
        ListNode *iter = temp;
        int carry = 0;
        ListNode *currl1 = l1;
        ListNode *currl2 = l2;
        
        while(currl1 != NULL || currl2 != NULL) {
            int val1 = 0;
            int val2 = 0;
            if(currl1 != NULL) val1 = currl1->val;
            if(currl2 != NULL) val2 = currl2->val;  

            int val = val1 + val2 + carry;
            carry = val/10;
            
            iter->next = new ListNode(val % 10);
            iter = iter->next;
        
            currl1 = (currl1 == NULL) ? currl1 : currl1->next;
            currl2 = (currl2 == NULL) ? currl2 : currl2->next;
        }
        if(carry > 0) {iter->next = new ListNode(1);}
        return temp->next;
        
    }
    
  
    
};