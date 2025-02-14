#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <bits/stdc++.h>

#include "./header/Shader.h"
#include "./header/Object.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void drawModel(const string& name, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, int r, int g, int b);
void init();

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;
float hY = 0.0f, hX = 0.0f;
// Shader
Shader *shader;

// Objects to display
Object *cube, *cylinder, *I, *C, *G, *tree_up, *tree_btn;

int main()
{
    // glfw: initialize and configures
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "ICG_2024_HW1", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);
    
    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // TODO: Enable depth test, face culling
    // depth test -> prevent occluded faces being rendered
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    // face culling -> reduce number of facese rendered by discarding unvisible faces
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);  // conterclockwise points are face
    glCullFace(GL_BACK);  // the plane it need to be culled


    // Display loop
	glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Initialize Object and Shader
    init();

    // I G rotate
    float rY = 0.0f;
    float rCon = 0.0f;
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // render
        glClearColor(153/255.0, 204/255.0, 255/255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        // Use shader
        shader->use();
        
        
        /*=================== Example of creating model matrix ======================= 
        1. translate
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 1.0f, 0.0f));
        drawModel("Cube",model,view,projection,255, 0, 0);
        
        2. scale
        glm::mat4 model(1.0f);
        model = glm::scale(model, glm::vec3(0.5f, 1.0f, 2.0f)); 
        drawModel("Cube",model,view,projection,255, 0, 0);
        
        3. rotate
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        drawModel("Cube",model,view,projection,255, 0, 0);
        ==============================================================================*/

        // TODO: Create model, view, and perspective matrix
        // view
        glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 50.0f, 90.0f),  // position
            glm::vec3(0.0f, 0.0f, 0.0f),  // target
            glm::vec3(0.0f, 1.0f, 0.0f)); // up = y-axis
        // perspective
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,   // near plane 
            1000.0f // far plane
        );

        // TODO: Helicopter (Body + Connector + Rotor Blades)
        // - Body (At least 3 different color cubes)
        glm::mat4 h1_model(1.0f);
        h1_model = glm::translate(h1_model, glm::vec3(hX+0.0f, hY+5.0f, 0.0f));
        glm::mat4 h1_scale = glm::scale(h1_model, glm::vec3(15.0f, 10.0f, 10.0f));
        drawModel("Cube", h1_scale, view, projection, 162, 181, 205);

        glm::mat4 h2_model = h1_model;
        h2_model = glm::translate(h2_model, glm::vec3(11.0f, -2.0f, 0.0f));
        h2_model = glm::scale(h2_model, glm::vec3(10.0f, 4.0f, 4.0f));
        drawModel("Cube", h2_model, view, projection, 181, 181, 181);

        glm::mat4 h3_model = h1_model;
        h3_model = glm::translate(h3_model, glm::vec3(18.2f, 0.2f, 0.0f));
        h3_model = glm::rotate(h3_model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        h3_model = glm::scale(h3_model, glm::vec3(10.0f, 3.0f, 3.0f));
        drawModel("Cube", h3_model, view, projection, 110, 123, 139);

        glm::mat4 w_model = h1_model;
        w_model = glm::translate(w_model, glm::vec3(-3.5f, 1.5f, 5.0f));
        w_model = glm::scale(w_model, glm::vec3(4.0f, 3.5f, 1.0f));
        drawModel("Cube", w_model, view, projection, 176, 226, 255);
        // - Connector (One cube)
        //     - On the body
        //     - Rotate: 5 degrees/frame about +y axis
        glm::mat4 con_model = h1_model;
        con_model = glm::translate(con_model, glm::vec3(0.0f, 6.5f, 0.0f));
        con_model = glm::rotate(con_model, glm::radians(rCon), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 con_scale = glm::scale(con_model, glm::vec3(2.0f, 2.5f, 2.0f));
        drawModel("Cube", con_scale, view, projection, 180, 205, 205);

        // - Rotor Blades (4 different color cubes)
        //     - On the connector
        //     - Each cubes is offset by 90 degrees
        glm::mat4 b1_model = con_model;
        // b1_model = glm::translate(b1_model, glm::vec3(hX, hY, 0.0f));
        // b1_model = glm::rotate(b1_model, glm::radians(rCon), glm::vec3(0.0f, 1.0f, 0.0f));
        b1_model = glm::translate(b1_model, glm::vec3(6.0f, 0.0f, 0.0f));
        b1_model = glm::scale(b1_model, glm::vec3(13.0f, 1.0f, 2.0f));
        drawModel("Cube", b1_model, view, projection, 255, 225, 255);

        glm::mat4 b2_model = con_model;
        // b2_model = glm::translate(b2_model, glm::vec3(hX, hY, 0.0f));
        b2_model = glm::rotate(b2_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        b2_model = glm::translate(b2_model, glm::vec3(6.0f, 0.0f, 0.0f));
        b2_model = glm::scale(b2_model, glm::vec3(13.0f, 1.0f, 2.0f));
        drawModel("Cube", b2_model, view, projection, 193, 205, 193);

        glm::mat4 b3_model = con_model;
        // b3_model = glm::translate(b3_model, glm::vec3(hX, hY, 0.0f));
        b3_model = glm::rotate(b3_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        b3_model = glm::translate(b3_model, glm::vec3(6.0f, 0.0f, 0.0f));
        b3_model = glm::scale(b3_model, glm::vec3(13.0f, 1.0f, 2.0f));
        drawModel("Cube", b3_model, view, projection, 238, 238, 224);

        glm::mat4 b4_model = con_model;
        // b4_model = glm::translate(b4_model, glm::vec3(hX, hY, 0.0f));
        b4_model = glm::rotate(b4_model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        b4_model = glm::translate(b4_model, glm::vec3(6.0f, 0.0f, 0.0f));
        b4_model = glm::scale(b4_model, glm::vec3(13.0f, 1.0f, 2.0f));
        drawModel("Cube", b4_model, view, projection, 139, 131, 120);
        rCon += 5.0f;
        if(rCon >= 360.0f) rCon -= 360.0f;
        // You can be creative in design !!

        // TODO: Draw Ground (Scale: (100, 1, 120))
        glm::mat4 gr_model(1.0f);
        glm::mat4 gr_scale = glm::scale(gr_model, glm::vec3(100.0f, 1.0f, 120.0f));
        drawModel("Cube", gr_scale, view, projection, 148, 237, 144);

        // TODO: Draw Tree1 (Position: (-30, 0, -20) relative to the Ground, Scale: (4, 4, 4))
        glm::mat4 tr1_model = gr_model;
        tr1_model = glm::translate(tr1_model, glm::vec3(-30.0f, 0.0f, -20.0f));
        tr1_model = glm::scale(tr1_model, glm::vec3(4.0f, 4.0f, 4.0f));
        drawModel("Tree_down", tr1_model, view, projection, 139, 105, 20);
        // glm::mat4 tr1_top_model = tr1_model;
        // tr1_top_model = glm::translate(tr1_top_model, glm::vec3(0.0f, 5.0f, 0.0f));
        drawModel("Tree_up", tr1_model, view, projection, 102, 205, 0);
        // TODO: Draw Tree2 (Position: (30, 0, 20) relative to the Ground, Scale: (3.5, 3.5, 3.5))
        glm::mat4 tr2_model = gr_model;
        tr2_model = glm::translate(tr2_model, glm::vec3(30.0f, 0.0f, 20.0f));
        tr2_model = glm::scale(tr2_model, glm::vec3(3.5f, 3.5f, 3.5f));
        drawModel("Tree_down", tr2_model, view, projection, 139, 105, 20);
        // glm::mat4 tr2_top_model = tr2_model;
        // tr2_top_model = glm::translate(tr2_top_model, glm::vec3(0.0f, 4.5f, 0.0f));
        drawModel("Tree_up", tr2_model, view, projection, 102, 205, 0);
        // TODO: Draw C
        glm::mat4 C_model(1.0f);
        C_model = glm::translate(C_model, glm::vec3(-25.0f, 2.0f, 25.0f));
        drawModel("C", C_model, view, projection, 238, 180, 180);
        // TODO: Draw I (Rotate: 0.8 degrees/ frame about +y axis around the C)
        glm::mat4 I_model = C_model;
        // I_model = glm::translate(I_model, glm::vec3(-25.0f, 2.0f, 25.0f));
        I_model = glm::rotate(I_model, glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f));
        I_model = glm::translate(I_model, glm::vec3(-12.0f, 0.0f, 0.0f));
        drawModel("I", I_model, view, projection, 238, 180, 180);
        // TODO: Draw G (Rotate: 0.8 degrees/ frame about +y axis around the C)
        glm::mat4 G_model = C_model;
        // G_model = glm::translate(G_model, glm::vec3(-25.0f, 2.0f, 25.0f));
        G_model = glm::rotate(G_model, glm::radians(rY), glm::vec3(0.0f, 1.0f, 0.0f));
        G_model = glm::translate(G_model, glm::vec3(12.0f, 0.0f, 0.0f));
        drawModel("G", G_model, view, projection, 238, 180, 180);
        // TODO: Control rotation
        rY += 0.8f;
        if(rY >= 360.0f) rY -= 360.0f;
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

// TODO:
// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
        
    // The action is one of GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE. 
    // Events with GLFW_PRESS and GLFW_RELEASE actions are emitted for every key press.
    // Most keys will also emit events with GLFW_REPEAT actions while a key is held down.
    // https://www.glfw.org/docs/3.3/input_guide.html

    // 1. Press W to move the helicopter in the Y direction by 1
    // 2. Press S to move the helicopter in the Y direction by -1 
    // 3. Press D to move the helicopter in the X direction by 1
    // 4. Press A to move the helicopter in the X direction by -1
    // 5. The helicopter cannot fly below the ground.
    // ** Both GLFW_PRESS and GLFW_REPEAT are OK! You can try it out and compare the differences. **
    if(key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        hY += 1;
    }
    else if(key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        hX -= 1;
    }
    else if(key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        if(hY > 0) hY -= 1;
    }
    else if(key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS)){
        hX += 1;
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void drawModel(const string& name, 
               const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection, 
               int r, int g, int b){

    shader->set_uniform("projection",projection);
    shader->set_uniform("view",view);
    shader->set_uniform("model",model);
    shader->set_uniform("objectColor", glm::vec3(r/255.0,g/255.0,b/255.0));
    
    if(name == "Cube"){
        cube->draw();
    }else if(name == "Cylinder"){
        cylinder->draw();
    }else if(name == "I"){
        I->draw();
    }else if(name == "C"){
        C->draw();
    }else if(name == "G"){
        G->draw();
    }else if(name == "Tree_down"){
        tree_btn->draw();
    }else if(name == "Tree_up"){
        tree_up->draw();
    }
}

void init(){
    #if defined(__linux__) || defined(__APPLE__)
        string dirShader = "../../src/shaders/";
        string dirAsset = "../../src/asset/";
    #else
        string dirShader = "..\\..\\src\\shaders\\";
        string dirAsset = "..\\..\\src\\asset\\";
    #endif

    string v = dirShader + "easy.vert";
    string f = dirShader + "easy.frag";
    shader = new Shader(v.c_str(),f.c_str());
    cube = new Object(dirAsset + "cube.obj");
    cylinder = new Object(dirAsset + "cylinder.obj");
    I = new Object(dirAsset + "I.obj");
    C = new Object(dirAsset + "C.obj");
    G = new Object(dirAsset + "G.obj");
    tree_up = new Object(dirAsset + "tree_up.obj");
    tree_btn = new Object(dirAsset + "tree_btn.obj");
}