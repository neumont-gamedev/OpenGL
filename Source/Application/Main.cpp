
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();
    
    SDL_Event e;
    bool quit = false;

    // OPENGL Initialization
    std::vector<glm::vec3> points{      { -0.5f, -0.5f, 0 }, { 0, 0.5f, 0 }, { 0.5f, -0.5f, 0 } };
    std::vector<glm::vec3> colors{      { 1, 0, 0 },         { 0, 1, 0 },    { 0, 0, 1 } };
    std::vector<glm::vec2> texcoord{    { 0, 0 },            { 0.5f, 1.0f }, { 1, 1 } };

    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texcoord;
    };

    std::vector<Vertex> vertices{
        { { -0.5f, -0.5f, 0 }, { 1, 0, 0 }, { 0, 0 } },
        { { -0.5f,  0.5f, 0 }, { 0, 1, 0 }, { 0, 1 } },
        { {  0.5f,  0.5f, 0 }, { 0, 0, 1 }, { 1, 1 } },
        { {  0.5f, -0.5f, 0 }, { 0, 0, 1 }, { 1, 0 } }
    };

    std::vector<GLushort> indices{ 0, 1, 2, 2, 3, 0 };

    // vertex buffer
    neu::res_t<neu::VertexBuffer> vb = std::make_shared<neu::VertexBuffer>();
    vb->CreateVertexBuffer((GLsizei)(sizeof(Vertex) * vertices.size()), (GLsizei)vertices.size(), vertices.data());
    vb->CreateIndexBuffer(GL_UNSIGNED_SHORT, (GLsizei)indices.size(), indices.data());
    vb->SetAttribute(0, 3, sizeof(Vertex), offsetof(Vertex, position));
    vb->SetAttribute(1, 3, sizeof(Vertex), offsetof(Vertex, color));
    vb->SetAttribute(2, 2, sizeof(Vertex), offsetof(Vertex, texcoord));

    // shaders
    auto vs = neu::Resources().Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
    auto fs = neu::Resources().Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);

    // program
    auto program = std::make_shared<neu::Program>();
    program->AttachShader(vs);
    program->AttachShader(fs);
    program->Link();
    program->Use();

    // texture
    neu::res_t<neu::Texture> texture = neu::Resources().Get<neu::Texture>("textures/beast.png");

    // uniform
    program->SetUniform("u_texture", 0);

    // transform
    float rotation = 0;
    glm::vec3 eye{ 0, 0, 5 };

    // projection matrix
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    program->SetUniform("u_projection", projection);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        // update
        neu::GetEngine().Update();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        rotation += neu::GetEngine().GetTime().GetDeltaTime() * 90;

        // model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
        program->SetUniform("u_model", model);

        // view matrix
        eye.x += neu::GetEngine().GetInput().GetMouseDelta().x * 0.01f;
        eye.z += neu::GetEngine().GetInput().GetMouseDelta().y * 0.01f;
        glm::mat4 view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        program->SetUniform("u_view", view);

        // draw
        neu::GetEngine().GetRenderer().Clear();
        
        vb->Draw(GL_TRIANGLES);

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
