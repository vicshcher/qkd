#ifndef QKDTOPOLOGY_HPP
#define QKDTOPOLOGY_HPP

#include <map>
#include <vector> 
#include <memory> 
#include <iostream>

#include "edge.hpp" 
#include "common.hpp"
#include "vertex.hpp"
#include "qkdrequestgen.hpp"

template <typename NetworkModel>
class QKD_Topology
{
    friend class Vertex;
    friend class Edge;

    template <NetworkModel>
        friend std::ostream& operator<< (std::ostream& os, 
                                         const QKD_Topology<NetworkModel>& t);

    private:

        NetworkModel& mQKD_Network;

        std::map< VertexId, std::shared_ptr<Vertex> > mmVertexToId;    
        std::map< EdgeId, std::shared_ptr<Edge> > mmEdgeToId;

        std::vector< VertexId > maRemovedVertexIdList;
        std::vector< EdgeId > maRemovedEdgeIdList;

    public:

        QKD_Topology ( NetworkModel& parent );

        VertexId addVertex();
        EdgeId addEdge( VertexId, VertexId );

        void removeVertex( VertexId );
        void removeEdge( EdgeId );

        Vertex& getVertexById( VertexId );
        Edge& getEdgeById( EdgeId );

        Vertex& getNewestVertex();
        Edge& getNewestEdge();
};

template < typename NetworkModel >
VertexId QKD_Topology< NetworkModel >::addVertex()
{
    Vertex* pv = new Vertex {};
    mmVertexToId.emplace( *pv.id, pv );
    msQKD_Network.mRequestGen.updMaxVertexId();
    return *pv.id;
}

template < typename NetworkModel >
EdgeId QKD_Topology< NetworkModel >::addEdge( VertexId v1, VertexId v2 )
{
    Edge* pe = new Edge { getVertexById( v1 ), getVertexById( v2 ) };
    mmEdgeToId.emplace( *pe.id, pe );
    mQKD_Network.mRequestGen.updMaxVertexId();
    return *pe.id; 
}

template < typename NetworkModel >
void QKD_Topology< NetworkModel >::removeVertex( VertexId v )
{
    for ( auto pe = mmEdgeToId.begin(); pe != mmEdgeToId.end(); ++pe )
        if ( pe->second->hasVertex( getVertexById( v ) ) ) 
        {
            maRemovedEdgeIdList.push_back( pe->first );
            mmEdgeToId.erase( pe );
            break;
        }

    auto pv = mmVertexToId.find( v );
    if( pv != mmVertexToId.end() )
    {
        maRemovedVertexIdList.push_back( pv->first );
        mmVertexToId.erase( pv );
    }
}

template < typename NetworkModel >
void QKD_Topology< NetworkModel >::removeEdge( EdgeId e )
{
    for ( auto p = mmEdgeToId.begin(); p != mmEdgeToId.end(); ++ p )
        if ( *( p->second ) == e ) 
        {
            -- p->second->num_adj_edges; 
            -- p->second->num_adj_edges; 
            maRemovedEdgeIdList.push_back( p->first );
            mmEdgeToId.erase( p );
            break;
        }
}

template < typename NetworkModel >
Vertex& QKD_Topology< NetworkModel >::getVertexById( VertexId id )
{
    return *mmVertexToId.at( id );
}

template < typename NetworkModel >
Edge& QKD_Topology< NetworkModel >::getEdgeById( EdgeId id )
{
    return *mmEdgeToId.at( id );
}
    
template < typename NetworkModel >
Vertex& getNewestVertex()
{
    return getVertexById( -- Vertex::last_vertex_id );    
}

template < typename NetworkModel >
Edge& getNewestEdge()
{
    return getEdgeById( -- Edge::last_vertex_id );    
}

template < typename NetworkModel >
std::ostream& operator<< ( std::ostream& os, 
                           const QKD_Topology< NetworkModel >& t )
{
    os << "Vertices: ";
    for ( const auto& [key, value] : t.mmVertexToId )
        os << *value << ", ";

    os << '\n';

    os << "Edges: ";
    for ( const auto& [key, value] : t.mmEdgeToId )
        os << *value << ", ";

    return os;
}

#endif  // QKDTOPOLOGY_HPP