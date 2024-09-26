class Solution(object):
    def updateMatrix(self, mat):
        """
        :type mat: List[List[int]]
        :rtype: List[List[int]]
        """
        bfs = []
        for n in range(len(mat)):
            for m in range(len(mat[0])):
                if mat[n][m] == 0:
                    bfs.append([n-1,m,1])
                    bfs.append([n+1,m,1])
                    bfs.append([n,m-1,1])
                    bfs.append([n,m+1,1])
                else:
                    mat[n][m] = -1

        while len(bfs) > 0:
            n,m,dist = bfs.pop(0)
            if n < 0 or n >= len(mat) or m < 0 or m >= len(mat[0]):
                continue

            if mat[n][m] == -1:
                mat[n][m] = dist
                bfs.append([n-1,m,dist+1])
                bfs.append([n+1,m,dist+1])
                bfs.append([n,m-1,dist+1])
                bfs.append([n,m+1,dist+1])
                
        return mat