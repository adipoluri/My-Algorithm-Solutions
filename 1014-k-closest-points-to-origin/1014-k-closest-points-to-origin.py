class Solution(object):
    def kClosest(self, points, k):
        """
        :type points: List[List[int]]
        :type k: int
        :rtype: List[List[int]]
        """
        heap = [(-self.euclidean_distance(points[i]),i) for i in range(k)]
        heapq.heapify(heap)

        for i in range(k, len(points)):
            dist = -self.euclidean_distance(points[i])
            if dist > heap[0][0]:
                heapq.heappushpop(heap, (dist, i))

        print(heap)
        return [points[i] for (_,i) in heap]

    def euclidean_distance(self,point):
        return point[0]**2 + point[1]**2