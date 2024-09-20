# Definition for a binary tree node.
# class TreeNode(object):
#     def __init__(self, x):
#         self.val = x
#         self.left = None
#         self.right = None

class Solution(object):
    def lowestCommonAncestor(self, root, p, q):
        """
        :type root: TreeNode
        :type p: TreeNode
        :type q: TreeNode
        :rtype: TreeNode
        """
        if root is None:
            return root

        lca = root.val
        if (p.val <= lca and q.val >= lca) or (p.val >= lca and q.val <= lca):
            return root
        elif lca > p.val and lca > q.val: # go down left tree
            return  self.lowestCommonAncestor(root.left, p,q)
        else:
            return self.lowestCommonAncestor(root.right, p,q)