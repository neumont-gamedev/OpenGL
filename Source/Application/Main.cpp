
int main(int argc, char* argv[]) {
    neu::file::SetCurrentDirectory("Assets");
    LOG_INFO("current directory {}", neu::file::GetCurrentDirectory());

    // initialize engine
    LOG_INFO("initialize engine...");
    neu::GetEngine().Initialize();
    
    SDL_Event e;
    bool quit = false;

    // OPENGL Initialization
    // model
    auto model3d = std::make_shared<neu::Model>();
    model3d->Load("models/spot.obj");

    // material
    auto material = neu::Resources().Get<neu::Material>("materials/spot.mat");
    material->Bind();

    // lights
    material->program->SetUniform("u_ambient_light", glm::vec3{ 0.1f });
    neu::Transform light{ { 2, 4, 0 } };
    glm::vec3 lightColor{ 1 };

    // transform
    neu::Transform transform{ { 0, 0, 0 } };
    neu::Transform camera{ { 0, 0, 3 } };

    // projection matrix
    float aspect = neu::GetEngine().GetRenderer().GetWidth() / (float)neu::GetEngine().GetRenderer().GetHeight();
    glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspect, 0.01f, 100.0f);
    material->program->SetUniform("u_projection", projection);

    // MAIN LOOP
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
            ImGui_ImplSDL3_ProcessEvent(&e);
        }

        // update
        neu::GetEngine().Update();
        float dt = neu::GetEngine().GetTime().GetDeltaTime();

        if (neu::GetEngine().GetInput().GetKeyPressed(SDL_SCANCODE_ESCAPE)) quit = true;

        material->program->SetUniform("u_model", transform.GetMatrix());

        // view matrix
        float speed = 10.0f;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_A)) camera.position.x -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_D)) camera.position.x += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_Q)) camera.position.y -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_E)) camera.position.y += speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_W)) camera.position.z -= speed * dt;
        if (neu::GetEngine().GetInput().GetKeyDown(SDL_SCANCODE_S)) camera.position.z += speed * dt;

        glm::mat4 view = glm::lookAt(camera.position, camera.position + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });
        material->program->SetUniform("u_view", view);

        material->program->SetUniform("u_light.color", lightColor);
        //light.position.x = neu::math::sin(neu::GetEngine().GetTime().GetTime() * 5) * 5;
        material->program->SetUniform("u_light.position", (glm::vec3)(view * glm::vec4(light.position, 1)));

        // draw
        neu::GetEngine().GetRenderer().Clear();

        // start new ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        // set ImGui
        ImGui::Begin("Editor");
        ImGui::ColorEdit3("Color", glm::value_ptr(lightColor));
        //light.UpdateGui();
        transform.UpdateGui();
        material->UpdateGui();
        ImGui::End();

        material->Bind();
        model3d->Draw(GL_TRIANGLES);

        // draw ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        neu::GetEngine().GetRenderer().Present();
    }

    neu::GetEngine().Shutdown();

    return 0;
}
