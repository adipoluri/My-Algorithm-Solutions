# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, val=0, left=None, right=None):
#         self.val = val
#         self.left = left
#         self.right = right
class Solution(object):
    def diameterOfBinaryTree(self, root):
        """
        :type root: TreeNode
        :rtype: int
        """
        return self.diameterOfBinaryTreedfs(root)[1]
    
    def diameterOfBinaryTreedfs(self, root):
        if root is None:
            return [0,0] # [max height, left max + right max]

        left = self.diameterOfBinaryTreedfs(root.left)
        right = self.diameterOfBinaryTreedfs(root.right)
        height = max(left[0],right[0])+1
        diameter = max(left[0]+right[0],left[1],right[1])
        return [height, diameter]