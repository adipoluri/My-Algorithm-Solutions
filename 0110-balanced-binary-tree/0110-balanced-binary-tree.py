# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution(object):
    def isBalanced(self, root):
        """
        :type root: TreeNode
        :rtype: bool
        """
        if root is None:
            return True
        
        return self.dfs(root)[1]

    def dfs(self, root):
        if root is None:
            return (0,True)
        left = self.dfs(root.left)
        right = self.dfs(root.right)
        
        check = abs(left[0] - right[0])
        return(max(left[0]+1,right[0]+1),check <= 1 and check >= 0 and left[1] and right[1])