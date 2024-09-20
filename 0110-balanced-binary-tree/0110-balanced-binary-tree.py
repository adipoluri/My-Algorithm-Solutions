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

        heightDiff = abs(self.getHeightofBtree(root.left) - self.getHeightofBtree(root.right))
        return heightDiff <= 1 and heightDiff >= 0 and self.isBalanced(root.left) and self.isBalanced(root.right)

    def getHeightofBtree(self, root):
        if root is None:
            return 0
        return(max(self.getHeightofBtree(root.left)+1,self.getHeightofBtree(root.right)+1))