
#include "RandomGraph.h"
#include "Graph.h"
#include "GraphTestSample.h"
#include "SNNCluster.h"
#include "gtest/gtest.h"

/* testGraphFunctionalities test case is about the behaviour of various graph operations */

/* Checks nodes creation */
TEST(testGraphFunctionalities, testNodesCreations)
{
	std::size_t count = 100;
	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(count, 0.5f);
	ASSERT_EQ(count, rg->getNodesCount());
	delete rg;
}

/* Checks edge creation */
TEST(testGraphFunctionalities, testLink)
{
	std::size_t count = 10;
	std::size_t expected = (count*count-count)/2;

	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(count, 1.0f);
	ASSERT_EQ(expected, rg->getEdgesCount());
	delete rg;
}

/* Checks edge link */
/* NOTE: Something terribly creepy about this test, it is not working fine */
TEST(testGraphFunctionalities, testUnlink)
{
	std::size_t count = 10;
	std::size_t toremove = 5;
	std::size_t removed = 0;
	std::size_t expected = 0;
	std::size_t edgesCount  = 0;

	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(10, 1.0f, true);
	
	edgesCount = rg->getEdgesCount();

	for(std::size_t i = 1; i < toremove+1; i++){
		if (rg->unlink(0, i)){
			removed++;
		}
	}

	expected = edgesCount - removed;

	ASSERT_EQ(expected, rg->getEdgesCount());
	delete rg;
}

TEST (testGraphFunctionalities, testAdjMatrix)
{
	std::size_t count = 20;
	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(count, 1.0f, false);
	
	rg->calculateAjdMatrix();

	for(std::size_t i=1; i<count;i++)
	{
		for(std::size_t j=0; j<i;j++)
		{
			ASSERT_EQ(true, rg->get(i, j));
			ASSERT_EQ(true, rg->get(j, i));
		}
		ASSERT_EQ(false, rg->get(i, i));
	}

	delete rg;
}

/* Test if both algorithms gives a same correct result with a very small error threshold */
TEST (testGraphFunctionalities, testTwoRPAlgorithms)
{
	GraphLib::Graph *g1 = new GraphLib::Graph(), 
		*g2 = new GraphLib::Graph(),
		*g3 = new GraphLib::Graph(false),
		*g4 = new GraphLib::Graph(false);

	std::size_t count = 5;
	
	for(std::size_t i=0; i<count; i++)
	{
		g1->createNode();
		g2->createNode();
		g3->createNode();
		g4->createNode();
	}

	auto n = g1->createNode();

	g1->link(0, 1);
	g1->link(0, 3);
	g1->link(1, 2);
	g1->link(0, 5);
	g1->link(1, 4);
	g1->link(3, 4);

	g2->link(0, 1);
	g2->link(0, 3);
	g2->link(1, 2);
	g2->link(1, 4);
	g2->link(3, 4);
	
	g3->link(0, 1);
	g3->link(0, 3);
	g3->link(1, 2);
	g3->link(1, 4);
	g3->link(3, 4);
	
	g4->link(0, 1);
	g4->link(0, 3);
	g4->link(1, 2);
	g4->link(1, 4);
	g4->link(3, 4);

	g1->removeNode(5);

	g1->calculateAjdMatrix();
	g2->calculateAjdMatrix();
	g3->calculateAjdMatrix();
	g4->calculateAjdMatrix();

	g1->calculatePageRank();
	g2->calculatePageRankMatrix();
	g3->calculatePageRank();
	g4->calculatePageRankMatrix();

	double sum1 = 0, sum2 = 0;

	ASSERT_NEAR(0.15f, g1->getRank(0), 0.00001f);
	ASSERT_NEAR(0.21375f, g1->getRank(1), 0.00001f);
	ASSERT_NEAR(0.2408438f, g1->getRank(2), 0.00001f);
	ASSERT_NEAR(0.21375f, g1->getRank(3), 0.00001f);
	ASSERT_NEAR(0.4225313f, g1->getRank(4), 0.00001f);
	
	
	ASSERT_NEAR(0.15f, g2->getRank(0), 0.00001f);
	ASSERT_NEAR(0.21375f, g2->getRank(1), 0.00001f);
	ASSERT_NEAR(0.2408438f, g2->getRank(2), 0.00001f);
	ASSERT_NEAR(0.21375f, g2->getRank(3), 0.00001f);
	ASSERT_NEAR(0.4225313f, g2->getRank(4), 0.00001f);

	ASSERT_NEAR(0.986941f, g3->getRank(0), 0.00001f);
	ASSERT_NEAR(1.4705596f, g3->getRank(1), 0.00001f);
	ASSERT_NEAR(0.5666586f, g3->getRank(2), 0.00001f);
	ASSERT_NEAR(0.9888998f, g3->getRank(3), 0.00001f);
	ASSERT_NEAR(0.986941f, g3->getRank(4), 0.00001f);
	
	ASSERT_NEAR(0.986941f, g4->getRank(0), 0.00001f);
	ASSERT_NEAR(1.4705596f, g4->getRank(1), 0.00001f);
	ASSERT_NEAR(0.5666586f, g4->getRank(2), 0.00001f);
	ASSERT_NEAR(0.9888998f, g4->getRank(3), 0.00001f);
	ASSERT_NEAR(0.986941f, g4->getRank(4), 0.00001f);

	delete g1;
	delete g2;	
	delete g3;
	delete g4;
}

/* Next test case "testSpeed" is about performance between iterativ and matrix page rank calculation methods */

TEST (testSpeed, testIterativ)
{
	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(1000, 0.5f);
	rg->calculateAjdMatrix();
	rg->calculatePageRank();
	delete rg;
}

TEST (testSpeed, testMatrix)
{
	GraphLib::RandomGraph *rg = new GraphLib::RandomGraph(50, 0.5f);
	rg->calculateAjdMatrix();
	rg->calculatePageRankMatrix();
	//rg->save(L"graph.csv");
	delete rg;
}

TEST (facebookDataTest, facebookTest1)
{
	GraphLib::GraphTestSample fdt(L"../Data/facebook/facebook_combined.txt");
	fdt.run();
	fdt.saveDOT(L"fb_graph.dot");

	SNNCluster::start(&fdt, 50);
	fdt.saveDOT(L"fb_cluster_50.dot");

	SNNCluster::start(&fdt, 100);
	fdt.saveDOT(L"fb_cluster_100.dot");

	SNNCluster::start(&fdt, 200);
	fdt.saveDOT(L"fb_cluster_200.dot");
}
