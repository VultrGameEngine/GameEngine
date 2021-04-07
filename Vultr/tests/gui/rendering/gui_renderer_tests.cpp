#include <gtest/gtest.h>
// Ugly but it works
#define private public
#define protected public
#include <gui/rendering/gui_renderer.h>
#include <Vultr.hpp>

class GUIRendererTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        Vultr::Engine *engine = new Vultr::Engine();
        engine->Init(false);
        renderer = new Vultr::GUI::GUIRenderer();
        vertex = Vultr::GUI::GUIVertex();
    }

    Vultr::GUI::GUIVertex vertex;
    Vultr::GUI::GUIRenderer *renderer;
};
TEST_F(GUIRendererTest, ShiftRightWorks)
{
    Vultr::GUI::GUIVertex vertex1 = Vultr::GUI::GUIVertex();
    vertex1.color = glm::vec4(1, 1, 1, 1);
    Vultr::GUI::GUIVertex vertex2 = Vultr::GUI::GUIVertex();
    vertex2.color = glm::vec4(2, 2, 2, 2);
    Vultr::GUI::GUIVertex vertices[] = {
        vertex1, vertex1, vertex1, vertex1, vertex2, vertex2, vertex2, vertex2,
    };
    for (int i = 0; i < 8; i++)
    {
        renderer->vertices[i] = vertices[i];
    }
    renderer->num_vertices = 8;

    renderer->num_vertices = 12;
    renderer->quad_to_vertex[0] = 0;
    renderer->vertex_to_quad[0] = 0;
    renderer->quad_to_vertex[1] = 4;
    renderer->vertex_to_quad[4] = 1;
    renderer->ShiftVerticesRight(0, renderer->num_vertices - 1, 4);
    EXPECT_EQ(renderer->vertices[4].color.x, vertices[0].color.x);
    EXPECT_EQ(renderer->vertices[5].color.x, vertices[1].color.x);
    EXPECT_EQ(renderer->vertices[6].color.x, vertices[2].color.x);
    EXPECT_EQ(renderer->vertices[7].color.x, vertices[3].color.x);
    EXPECT_EQ(renderer->vertices[8].color.x, vertices[4].color.x);
    EXPECT_EQ(renderer->vertices[9].color.x, vertices[5].color.x);
    EXPECT_EQ(renderer->vertices[10].color.x, vertices[6].color.x);
    EXPECT_EQ(renderer->vertices[11].color.x, vertices[7].color.x);
    EXPECT_EQ(renderer->quad_to_vertex[0], 4);
    EXPECT_EQ(renderer->quad_to_vertex[1], 8);
    EXPECT_EQ(renderer->vertex_to_quad[4], 0);
    EXPECT_EQ(renderer->vertex_to_quad[8], 1);
}

TEST_F(GUIRendererTest, ShiftLeftWorks)
{
    Vultr::GUI::GUIVertex vertex1 = Vultr::GUI::GUIVertex();
    vertex1.color = glm::vec4(1, 1, 1, 1);
    Vultr::GUI::GUIVertex vertex2 = Vultr::GUI::GUIVertex();
    vertex2.color = glm::vec4(2, 2, 2, 2);
    Vultr::GUI::GUIVertex vertices[] = {
        vertex1, vertex1, vertex1, vertex1, vertex2, vertex2, vertex2, vertex2,
    };
    for (int i = 0; i < 8; i++)
    {
        renderer->vertices[i + 4] = vertices[i];
    }
    renderer->num_vertices = 12;
    renderer->quad_to_vertex[0] = 4;
    renderer->vertex_to_quad[4] = 0;
    renderer->quad_to_vertex[1] = 8;
    renderer->vertex_to_quad[8] = 1;

    renderer->ShiftVerticesLeft(4, 11, 4);
    EXPECT_EQ(renderer->vertices[0].color.x, vertices[0].color.x);
    EXPECT_EQ(renderer->vertices[1].color.x, vertices[1].color.x);
    EXPECT_EQ(renderer->vertices[2].color.x, vertices[2].color.x);
    EXPECT_EQ(renderer->vertices[3].color.x, vertices[3].color.x);
    EXPECT_EQ(renderer->vertices[4].color.x, vertices[4].color.x);
    EXPECT_EQ(renderer->vertices[5].color.x, vertices[5].color.x);
    EXPECT_EQ(renderer->vertices[6].color.x, vertices[6].color.x);
    EXPECT_EQ(renderer->vertices[7].color.x, vertices[7].color.x);
    EXPECT_EQ(renderer->quad_to_vertex[0], 0);
    EXPECT_EQ(renderer->quad_to_vertex[1], 4);
    EXPECT_EQ(renderer->vertex_to_quad[0], 0);
    EXPECT_EQ(renderer->vertex_to_quad[4], 1);
    delete renderer;
}

TEST_F(GUIRendererTest, SubmitNewVertexInZNewRangeWorks)
{
    vertex.position.z = -3.0 / 100;
    Vultr::GUI::GUIVertex vertices[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitNewVertexInZNewRange(3, vertices);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);
    EXPECT_DOUBLE_EQ(renderer->vertices[0].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[1].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[2].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[3].position.z * 100, -3);
    vertex.position.z = 0.0 / 100;
    Vultr::GUI::GUIVertex vertices2[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitNewVertexInZNewRange(0, vertices2);
    vertex.position.z = -1.0 / 100;
    Vultr::GUI::GUIVertex vertices3[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitNewVertexInZNewRange(1, vertices3);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 8);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);
    EXPECT_EQ(renderer->z_index_ranges[0].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[0].end, 3);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 4);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 7);
    EXPECT_EQ(renderer->vertices[0].position.z, 0);
    EXPECT_EQ(renderer->vertices[1].position.z, 0);
    EXPECT_EQ(renderer->vertices[2].position.z, 0);
    EXPECT_EQ(renderer->vertices[3].position.z, 0);
    EXPECT_EQ(renderer->vertices[4].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[5].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[6].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[7].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -3);
}

TEST_F(GUIRendererTest, SubmitVerticesToRangeWorks)
{
    renderer->z_index_ranges[3] = Vultr::GUI::VertexRange(8);
    renderer->z_index_ranges[0] = Vultr::GUI::VertexRange(0, 3);
    renderer->z_index_ranges[1] = Vultr::GUI::VertexRange(4, 7);

    vertex.position.z = 0.0 / 100;
    for (int i = 0; i < 4; i++)
    {
        renderer->vertices[i] = vertex;
    }

    vertex.position.z = -1.0 / 100;
    for (int i = 4; i < 8; i++)
    {
        renderer->vertices[i] = vertex;
    }
    vertex.position.z = -3.0 / 100;
    for (int i = 8; i < 12; i++)
    {
        renderer->vertices[i] = vertex;
    }

    vertex.position.z = 0.0 / 100;
    Vultr::GUI::GUIVertex vertices[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->num_vertices = 12;
    renderer->SubmitVerticesToRange(0, vertices);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 12);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);
    EXPECT_EQ(renderer->z_index_ranges[0].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[0].end, 7);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 8);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 11);
    EXPECT_EQ(renderer->vertices[0].position.z, 0);
    EXPECT_EQ(renderer->vertices[1].position.z, 0);
    EXPECT_EQ(renderer->vertices[2].position.z, 0);
    EXPECT_EQ(renderer->vertices[3].position.z, 0);
    EXPECT_EQ(renderer->vertices[4].position.z, 0);
    EXPECT_EQ(renderer->vertices[5].position.z, 0);
    EXPECT_EQ(renderer->vertices[6].position.z, 0);
    EXPECT_EQ(renderer->vertices[7].position.z, 0);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[12].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[13].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[14].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[15].position.z * 100, -3);

    vertex.position.z = -3.0 / 100;
    Vultr::GUI::GUIVertex vertices2[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitVerticesToRange(3, vertices2);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 12);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);
    EXPECT_EQ(renderer->z_index_ranges[0].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[0].end, 7);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 8);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 11);
    EXPECT_EQ(renderer->vertices[0].position.z, 0);
    EXPECT_EQ(renderer->vertices[1].position.z, 0);
    EXPECT_EQ(renderer->vertices[2].position.z, 0);
    EXPECT_EQ(renderer->vertices[3].position.z, 0);
    EXPECT_EQ(renderer->vertices[4].position.z, 0);
    EXPECT_EQ(renderer->vertices[5].position.z, 0);
    EXPECT_EQ(renderer->vertices[6].position.z, 0);
    EXPECT_EQ(renderer->vertices[7].position.z, 0);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[12].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[13].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[14].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[15].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[16].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[17].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[18].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[19].position.z * 100, -3);
}

TEST_F(GUIRendererTest, DeleteVerticesWorks)
{
    renderer->z_index_ranges[3] = Vultr::GUI::VertexRange(8);
    renderer->z_index_ranges[0] = Vultr::GUI::VertexRange(0, 3);
    renderer->z_index_ranges[1] = Vultr::GUI::VertexRange(4, 7);
    renderer->vertex_to_quad[4] = 1;
    renderer->quad_to_vertex[1] = 4;

    vertex.position.z = 0.0 / 100;
    for (int i = 0; i < 4; i++)
    {
        renderer->vertices[i] = vertex;
    }

    vertex.position.z = -1.0 / 100;
    for (int i = 4; i < 8; i++)
    {
        renderer->vertices[i] = vertex;
    }
    vertex.position.z = -3.0 / 100;
    for (int i = 8; i < 12; i++)
    {
        renderer->vertices[i] = vertex;
    }
    renderer->num_vertices = 12;
    renderer->DeleteVertices(4, 4);

    EXPECT_EQ(renderer->z_index_ranges[3].begin, 4);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);
    EXPECT_EQ(renderer->z_index_ranges[0].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[0].end, 3);
    EXPECT_EQ(renderer->num_vertices, 8);
    EXPECT_TRUE(renderer->z_index_ranges.find(1) == renderer->z_index_ranges.end());
    EXPECT_TRUE(renderer->quad_to_vertex.find(1) == renderer->quad_to_vertex.end());
    EXPECT_TRUE(renderer->vertex_to_quad.find(4) == renderer->vertex_to_quad.end());
    EXPECT_EQ(renderer->vertices[0].position.z, 0);
    EXPECT_EQ(renderer->vertices[1].position.z, 0);
    EXPECT_EQ(renderer->vertices[2].position.z, 0);
    EXPECT_EQ(renderer->vertices[3].position.z, 0);
    EXPECT_EQ(renderer->vertices[4].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[5].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[6].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[7].position.z * 100, -3);
}

TEST(GUIRendererTests, CommitVerticesWorks)
{
    Vultr::GUI::GUIRenderer *renderer = new Vultr::GUI::GUIRenderer();
    Vultr::GUI::GUIVertex vertex = Vultr::GUI::GUIVertex();
    vertex.position.z = -3.0 / 100;
    Vultr::GUI::GUIVertex vertices[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitNewVertexInZNewRange(3, vertices);
    vertex.position.z = -1.0 / 100;
    Vultr::GUI::GUIVertex vertices2[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->SubmitNewVertexInZNewRange(1, vertices2);
    renderer->SubmitVerticesToRange(1, vertices2);
    renderer->CommitVertices(0, vertices2);

    vertex.position.z = -2.0 / 100;
    Vultr::GUI::GUIVertex vertices3[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->CommitVertices(1, vertices3);
    EXPECT_EQ(renderer->quad_to_vertex[0], 8);
    EXPECT_EQ(renderer->vertex_to_quad[8], 0);
    EXPECT_EQ(renderer->quad_to_vertex[1], 12);
    EXPECT_EQ(renderer->vertex_to_quad[12], 1);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 11);
    EXPECT_EQ(renderer->z_index_ranges[2].begin, 12);
    EXPECT_EQ(renderer->z_index_ranges[2].end, 15);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 16);
    EXPECT_EQ(renderer->z_index_ranges[3].end, MAX_QUADS * 4);

    EXPECT_EQ(renderer->vertices[0].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[1].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[2].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[3].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[4].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[5].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[6].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[7].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[12].position.z * 100, -2);
    EXPECT_EQ(renderer->vertices[13].position.z * 100, -2);
    EXPECT_EQ(renderer->vertices[14].position.z * 100, -2);
    EXPECT_EQ(renderer->vertices[15].position.z * 100, -2);
    EXPECT_EQ(renderer->vertices[16].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[17].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[18].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[19].position.z * 100, -3);
    vertex.position.z = -4.0 / 100;
    Vultr::GUI::GUIVertex vertices4[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->CommitVertices(1, vertices4);
    EXPECT_EQ(renderer->quad_to_vertex[0], 8);
    EXPECT_EQ(renderer->vertex_to_quad[8], 0);
    EXPECT_EQ(renderer->quad_to_vertex[1], 16);
    EXPECT_EQ(renderer->vertex_to_quad[16], 1);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 11);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 12);
    EXPECT_EQ(renderer->z_index_ranges[3].end, 15);
    EXPECT_EQ(renderer->vertices[0].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[1].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[2].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[3].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[4].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[5].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[6].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[7].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[12].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[13].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[14].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[15].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[16].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[17].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[18].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[19].position.z * 100, -4);
    vertex.position.z = -3.0 / 100;
    Vultr::GUI::GUIVertex vertices5[] = {
        vertex,
        vertex,
        vertex,
        vertex,
    };
    renderer->CommitVertices(0, vertices5);
    EXPECT_EQ(renderer->quad_to_vertex[0], 12);
    EXPECT_EQ(renderer->vertex_to_quad[12], 0);
    EXPECT_EQ(renderer->quad_to_vertex[1], 16);
    EXPECT_EQ(renderer->vertex_to_quad[16], 1);
    EXPECT_EQ(renderer->z_index_ranges[1].begin, 0);
    EXPECT_EQ(renderer->z_index_ranges[1].end, 7);
    EXPECT_EQ(renderer->z_index_ranges[3].begin, 8);
    EXPECT_EQ(renderer->z_index_ranges[3].end, 15);
    EXPECT_EQ(renderer->vertices[0].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[1].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[2].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[3].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[4].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[5].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[6].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[7].position.z * 100, -1);
    EXPECT_EQ(renderer->vertices[8].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[9].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[10].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[11].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[12].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[13].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[14].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[15].position.z * 100, -3);
    EXPECT_EQ(renderer->vertices[16].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[17].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[18].position.z * 100, -4);
    EXPECT_EQ(renderer->vertices[19].position.z * 100, -4);
    delete renderer;
}
