#pragma once

#include<vector>

class Adjacency{
  protected:
    std::vector<unsigned>_edges;          ///< (a_ind,b_ind,offset,count)* 
    std::vector<unsigned>_opposite;       ///< list of all indices to opposite vertices
    unsigned             _maxMultiplicity;///<max allowed multiplicity
    const float*         _vertices;       ///<all vertices, with redundancies
    unsigned             _nofTriangles;   ///<number of triangles
  public:
    /**
     * @brief constructor of adjacency information
     *
     * @param points vertices, three consecutive vertices form triangle
     * @param numTriangles number of triangles
     * @param maxMultiplicity edges with larger nof opposite vertices will be split
     */
    Adjacency(const float*vertices,unsigned nofTriangles,unsigned maxMultiplicity);
    /**
     * @brief gets number of edges
     *
     * @return 
     */
    unsigned getNofEdges();
    /**
     * @brief gets indices of edge vertices
     *
     * @param e edge number
     * @param i 0 - vertex a, 1 - vertex b of edge e
     *
     * @return index of vertex a or b of edge e
     */
    unsigned getEdge       (unsigned e,unsigned i);
    /**
     * @brief gets number of opposite vertices of edge e
     *
     * @param e edge number
     *
     * @return number of opposite vertice of edge e
     */
    unsigned getNofOpposite(unsigned e);
    /**
     * @brief gets index of opposite vertex
     *
     * @param e edge e
     * @param i ith opposite vertex
     *
     * @return index of ith opposite vertex of edge e
     */
    unsigned getOpposite   (unsigned e,unsigned i);
    /**
     * @brief gets maximal multiplicity
     *
     * @return maximal multiplicity
     */
    unsigned getMaxMultiplicity();
    /**
     * @brief gets array of vertices
     *
     * @return array of vertices
     */
    const float*getVertices();
    /**
     * @brief gets nof triangles
     *
     * @return number of triangles
     */
    unsigned getNofTriangles();
};