#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <bits/stdc++.h>

#include "./header/Object.h"
#include "./header/stb_image.h"

typedef struct Render{
    string modelName;
    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
    bool useRainbow;
    bool useSqueeze;
    bool useHeli;
    glm::vec3 color;
    GLint modelLoc, viewLoc, projLoc, squeezLoc, rainbowLoc, textureLoc, useRainbowColorLoc, useSqueezeLoc, useHeliLoc;
}Render;

void framebufferSizeCallback(GLFWwindow *window, int width, int height);
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
unsigned int createShader(const string &filename, const string &type);
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader);
unsigned int modelVAO(Object &model);
unsigned int loadTexture(const char *tFileName);
glm::vec3 hsv_rgb(float H);
void drawModel(Render renPara);
void init();

// settings
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

// Shader
unsigned int vertexShader, fragmentShader, shaderProgram;

// Texture
unsigned int airplaneTexture, earthTexture;

// VAO, VBO
unsigned int airplaneVAO, earthVAO, cubeVAO;

// Objects to display
Object *airplaneObject, *earthObject, *cubeObject;

// Constants you may need
const int airplaneHeight = 27;
const int rotateEarthSpeed = 30;
const int rotateAirplaneSpeed = 90;
const int squeezeSpeed = 90;
const int rainbowSpeed = 72;

// You can use these parameters
float rotateEarthDegree = 0;
float rotateAirplaneDegree = 0;
int rotateAxisDegree = 180;
bool useSqueeze = false;
float squeezeFactor = 0;
bool useRainbowColor = false;
float rainbowDegree = 0;
glm::vec3 rainbowColor = glm::vec3(1.0f, 1.0f, 1.0f);
bool useHeli = false;

int main() {
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    /* TODO#0: Change window title to "HW2 - [your student id]"
     *        Ex. HW2 - 312550000
     */

    // glfw window creation
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HW2 - 111550139", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSwapInterval(1);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // TODO#1: Finish function createShader
    // TODO#2: Finish function createProgram
    // TODO#3: Finish function modelVAO
    // TODO#4: Finish function loadTexture
    // You can find the above functions right below the main function

    // Initialize Object, Shader, Texture, VAO, VBO
    init();

    // Enable depth test, face culling
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    // Set viewport
    glfwGetFramebufferSize(window, &SCR_WIDTH, &SCR_HEIGHT);
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // render loop variables
    double dt;
    double lastTime = glfwGetTime();
    double currentTime;

    /* TODO#5: Data connection - Retrieve uniform variable locations
     *    1. Retrieve locations for model, view, and projection matrices.
     *    2. Retrieve locations for squeezeFactor, rainbowColor, and other parameters.
     * Hint:
     *    glGetUniformLocation
     */
    Render renPara;
    renPara.modelLoc = glGetUniformLocation(shaderProgram, "model");
    renPara.viewLoc = glGetUniformLocation(shaderProgram, "view");
    renPara.projLoc = glGetUniformLocation(shaderProgram, "projection");
    renPara.squeezLoc = glGetUniformLocation(shaderProgram, "squeezeFactor");
    renPara.rainbowLoc = glGetUniformLocation(shaderProgram, "rainbowColor");
    renPara.textureLoc = glGetUniformLocation(shaderProgram, "ourTexture");
    renPara.useRainbowColorLoc = glGetUniformLocation(shaderProgram, "useRainbowColor");
    renPara.useSqueezeLoc = glGetUniformLocation(shaderProgram, "useSqueeze");
    renPara.useHeliLoc = glGetUniformLocation(shaderProgram, "useHeli");
    
    // heli rotate
    float rCon = 0.0f;

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // render
        glClearColor(0 / 255.0, 0 / 255.0, 0 / 255.0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 50.0f, 90.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);

        glm::mat4 base(1.0f), airplaneModel(1.0f), earthModel(1.0f);

        renPara.view = view;
        renPara.projection = projection;
        /* TODO#6-1: Render Airplane
         *    1. Set up airplane model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the airplane.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */
        // string dirShader = "..\\..\\src\\shaders\\";
        airplaneModel = glm::rotate(airplaneModel, glm::radians((float)rotateAxisDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        airplaneModel = glm::rotate(airplaneModel, glm::radians(rotateAirplaneDegree), glm::vec3(-1.0f, 0.0f, 0.0f));
        airplaneModel = glm::translate(airplaneModel, glm::vec3(0.0f, airplaneHeight, 0.0f));
        // send matrices to shader
        if (!useHeli) {
            renPara.useHeli = 0;
            renPara.useRainbow = (useRainbowColor? 1: 0);
            renPara.useSqueeze = 0;
            renPara.model = airplaneModel;
            renPara.modelName = "plane";
            renPara.color = rainbowColor;
            drawModel(renPara);
        }
        else {   // use Helicopter
            // - setup default renPara
            renPara.useHeli = 1;
            renPara.useRainbow = 0;
            renPara.useSqueeze = 0;
            renPara.modelName = "cube";

            glm::mat4 h1_model = airplaneModel;
            h1_model = glm::translate(h1_model, glm::vec3(0.0f, 5.0f, 0.0f));
            glm::mat4 h1_scale = glm::scale(h1_model, glm::vec3(10.0f, 10.0f, 15.0f));
            renPara.model = h1_scale;
            renPara.color = glm::vec3(162/255.0f, 181/255.0f, 205/255.0f);
            drawModel(renPara);

            glm::mat4 h2_model = h1_model;
            h2_model = glm::translate(h2_model, glm::vec3(0.0f, -2.0f, 11.0f));
            h2_model = glm::scale(h2_model, glm::vec3(4.0f, 4.0f, 10.0f));
            renPara.model = h2_model;
            renPara.color = glm::vec3(181/255.0f, 181/255.0f, 181/255.0f);
            drawModel(renPara);

            glm::mat4 h3_model = h1_model;
            h3_model = glm::translate(h3_model, glm::vec3(0.0f, 0.2f, 18.2f));
            h3_model = glm::rotate(h3_model, glm::radians(135.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            h3_model = glm::scale(h3_model, glm::vec3(3.0f, 3.0f, 10.0f));
            renPara.model = h3_model;
            renPara.color = glm::vec3(110/255.0f, 123/255.0f, 139/255.0f);
            drawModel(renPara);
            
            glm::mat4 w_model = h1_model;
            w_model = glm::translate(w_model, glm::vec3(5.0f, 1.5f, -3.5f));
            w_model = glm::scale(w_model, glm::vec3(1.0f, 3.5f, 4.0f));
            renPara.model = w_model;
            renPara.color = glm::vec3(176/255.0f, 226/255.0f, 255/255.0f);
            drawModel(renPara);
            
            glm::mat4 w1_model = h1_model;
            w1_model = glm::translate(w1_model, glm::vec3(-5.0f, 1.5f, -3.5f));
            w1_model = glm::scale(w1_model, glm::vec3(1.0f, 3.5f, 4.0f));
            renPara.model = w1_model;
            renPara.color = glm::vec3(176/255.0f, 226/255.0f, 255/255.0f);
            drawModel(renPara);
            
            // // - Connector (One cube)
            // //     - On the body
            // //     - Rotate: 5 degrees/frame about +y axis
            glm::mat4 con_model = h1_model;
            con_model = glm::translate(con_model, glm::vec3(0.0f, 6.5f, 0.0f));
            con_model = glm::rotate(con_model, glm::radians(rCon), glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 con_scale = glm::scale(con_model, glm::vec3(2.0f, 2.5f, 2.0f));
            renPara.model = con_scale;
            renPara.color = glm::vec3(180/255.0f, 205/255.0f, 205/255.0f);
            drawModel(renPara);

            // // - Rotor Blades (4 different color cubes)
            // //     - On the connector
            // //     - Each cubes is offset by 90 degrees
            glm::mat4 b1_model = con_model;
            // b1_model = glm::translate(b1_model, glm::vec3(hX, hY, 0.0f));
            // b1_model = glm::rotate(b1_model, glm::radians(rCon), glm::vec3(0.0f, 1.0f, 0.0f));
            b1_model = glm::translate(b1_model, glm::vec3(6.0f, 0.0f, 0.0f));
            b1_model = glm::scale(b1_model, glm::vec3(13.0f, 1.0f, 2.0f));
            renPara.model = b1_model;
            renPara.color = glm::vec3(255/255.0f, 225/255.0f, 255/255.0f);
            drawModel(renPara);

            glm::mat4 b2_model = con_model;
            // b2_model = glm::translate(b2_model, glm::vec3(hX, hY, 0.0f));
            b2_model = glm::rotate(b2_model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            b2_model = glm::translate(b2_model, glm::vec3(6.0f, 0.0f, 0.0f));
            b2_model = glm::scale(b2_model, glm::vec3(13.0f, 1.0f, 2.0f));
            renPara.model = b2_model;
            renPara.color = glm::vec3(193/255.0f, 205/255.0f, 193/255.0f);
            drawModel(renPara);

            glm::mat4 b3_model = con_model;
            // b3_model = glm::translate(b3_model, glm::vec3(hX, hY, 0.0f));
            b3_model = glm::rotate(b3_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            b3_model = glm::translate(b3_model, glm::vec3(6.0f, 0.0f, 0.0f));
            b3_model = glm::scale(b3_model, glm::vec3(13.0f, 1.0f, 2.0f));
            renPara.model = b3_model;
            renPara.color = glm::vec3(238/255.0f, 238/255.0f, 224/255.0f);
            drawModel(renPara);

            glm::mat4 b4_model = con_model;
            // b4_model = glm::translate(b4_model, glm::vec3(hX, hY, 0.0f));
            b4_model = glm::rotate(b4_model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            b4_model = glm::translate(b4_model, glm::vec3(6.0f, 0.0f, 0.0f));
            b4_model = glm::scale(b4_model, glm::vec3(13.0f, 1.0f, 2.0f));
            renPara.model = b4_model;
            renPara.color = glm::vec3(139/255.0f, 131/255.0f, 120/255.0f);
            drawModel(renPara);
        }
        

        /* TODO#6-2: Render Earth
         *    1. Set up earth model matrix.
         *    2. Send model, view, and projection matrices to the program.
         *    3. Send squeezeFactor, rainbowColor, or other parameters to the program.
         *    4. Apply the texture, and render the earth.
         * Hint:
         *	  rotate, translate, scale
         *    glUniformMatrix4fv, glUniform1f, glUniform3fv
         *    glActiveTexture, glBindTexture, glBindVertexArray, glDrawArrays
         */
        earthModel = glm::rotate(earthModel, glm::radians(rotateEarthDegree), glm::vec3(0.0f, 1.0f, 0.0f));
        earthModel = glm::scale(earthModel, glm::vec3(10.0f, 10.0f, 10.0f));
        renPara.useHeli = 0;
        renPara.useRainbow = 0;
        renPara.useSqueeze = (1);
        renPara.model = earthModel;
        renPara.modelName = "earth";
        renPara.color = rainbowColor;
        drawModel(renPara);

        // Status update
        currentTime = glfwGetTime();
        dt = currentTime - lastTime;
        lastTime = currentTime;

        /* TODO#7: Update "rotateEarthDegree", "rotateAirplaneDegree", "rotateAxisDegree",
         *          "squeezeFactor", "rainbowColor"
         */
        if (rotateEarthDegree + rotateEarthSpeed*dt >= 360) rotateEarthDegree -= 360;
        rotateEarthDegree += rotateEarthSpeed*dt;
        if (rotateAirplaneDegree + rotateAirplaneSpeed*dt >= 360) rotateAirplaneDegree -= 360;
        rotateAirplaneDegree += rotateAirplaneSpeed*dt;
        
        if (useSqueeze) {
            if (squeezeFactor + squeezeSpeed*dt >= 360) squeezeFactor -= 360;
            squeezeFactor += squeezeSpeed*dt;
            // squeezey += squeezez * sin(glm::radians(squeezeFactor)) / 2;
            // squeezez += squeezey * sin(glm::radians(squeezeFactor)) / 2;
            // cout << "now squeeze y, z = " << squeezey << " " << squeezez << "\n";
        }

        if (useRainbowColor) {
            if (rainbowDegree + rainbowSpeed*dt >= 360) rainbowDegree -= 360;
            rainbowDegree += rainbowSpeed*dt;
            rainbowColor = hsv_rgb(rainbowDegree);
        }
        else {
            rainbowDegree = 0;
            rainbowColor = glm::vec3(1.0f, 1.0f, 1.0f);
        }

        rCon += 5.0f;
        if(rCon >= 360.0f) rCon -= 360.0f;
        
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

/* TODO: convert hsv to rgb */
glm::vec3 hsv_rgb(float H) {
    float X = 1 - fabs(fmod(H / 60.0f, 2) - 1);
    float r, g, b;
    if (H >= 0 && H < 60) {
        r = 1; g = X; b = 0;
    } else if (H >= 60 && H < 120) {
        r = X; g = 1; b = 0;
    } else if (H >= 120 && H < 180) {
        r = 0; g = 1; b = X;
    } else if (H >= 180 && H < 240) {
        r = 0; g = X; b = 1;
    } else if (H >= 240 && H < 300) {
        r = X; g = 0; b = 1;
    } else {
        r = 1; g = 0; b = X;
    }
    return glm::vec3(r, g, b);
}

/* TODO#1: createShader
 * input:
 *      filename: shader file name
 *      type: shader type, "vert" means vertex shader, "frag" means fragment shader
 * output: shader object
 * Hint:
 *      glCreateShader, glShaderSource, glCompileShader
 */
unsigned int createShader(const string &filename, const string &type) {
    GLuint shader;
    if (type == "frag")
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    else if (type == "vert")
        shader = glCreateShader(GL_VERTEX_SHADER);
    
    std::ifstream ShaderFile;
    std::stringstream ShaderStream;
    std::string code;
    try {
        ShaderFile.open(filename);
        ShaderStream << ShaderFile.rdbuf();
        ShaderFile.close();
        code = ShaderStream.str();
    }
    catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << "\n";
    }
    const char* shaderCode = code.c_str();
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    // debug
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        std::cout << "Shader code:\n" << code << std::endl;
    } else {
        std::cout << "Shader " << type << " compiled successfully" << std::endl;
    }

    return shader;
}

/* TODO#2: createProgram
 * input:
 *      vertexShader: vertex shader object
 *      fragmentShader: fragment shader object
 * output: shader program
 * Hint:
 *      glCreateProgram, glAttachShader, glLinkProgram, glDetachShader
 */
unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader) {
    GLuint ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // debug
    int success;
    char infoLog[512];
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    } else {
        std::cout << "Shader program linked successfully" << std::endl;
    }

    // check valid
    glValidateProgram(ID);
    glGetProgramiv(ID, GL_VALIDATE_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::VALIDATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return ID;
}

/* TODO#3: modelVAO
 * input:
 *      model: Object you want to render
 * output: VAO
 * Hint:
 *      glGenVertexArrays, glBindVertexArray, glGenBuffers, glBindBuffer, glBufferData,
 *      glVertexAttribPointer, glEnableVertexAttribArray,
 */
unsigned int modelVAO(Object &model) {
    vector<float> positions = model.positions;
    vector<float> normals = model.normals;
    vector<float> texcoords = model.texcoords;

    unsigned int VBO[3];
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(3, VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)  * (positions.size()), &(positions[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)  * (normals.size()), &(normals[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)  * (texcoords.size()), &(texcoords[0]), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    // glDrawArrays();
    return VAO;
}

/* TODO#4: loadTexture
 * input:
 *      filename: texture file name
 * output: texture object
 * Hint:
 *      glEnable, glGenTextures, glBindTexture, glTexParameteri, glTexImage2D
 */
unsigned int loadTexture(const string &filename) {
    GLuint texture;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    if (filename == "..\\..\\src\\asset\\texture\\earth.jpg") stbi_set_flip_vertically_on_load(1);
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        std::cout << "texture loaded successfully: " << filename << std::endl;
        std::cout << "width: " << width << ", height: " << height << ", channels: " << nrChannels << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    else {
        std::cout << "ERROR::TEXTURE::FAILED_TO_LOAD_TEXTURE: " << filename << std::endl;
        stbi_image_free(data);
    }
    return texture;
}

/* TODO#8: Key callback
 *    1. Press 'd' to increase the "rotateAxisDegree" by 1.
 *    2. Press 'a' to decrease the "rotateAxisDegree" by 1.
 *    3. Press 's' to squeeze the earth.
 *    4. Press 'r' to make the color of the airplane rainbow.
 * Hint:
 *      GLFW_KEY_D, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_R
 *      GLFW_PRESS, GLFW_REPEAT
 */
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        rotateAxisDegree += 1;
        if (rotateAxisDegree >= 360) rotateAxisDegree -= 360;
    }
    if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        rotateAxisDegree -= 1;
        if (rotateAxisDegree < 0) rotateAxisDegree += 360;
    }
    if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if(useSqueeze) useSqueeze = 0;
        else useSqueeze = 1;
    }
    if (key == GLFW_KEY_R && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if (useRainbowColor) useRainbowColor = 0;
        else useRainbowColor = 1;
    }
    if (key == GLFW_KEY_H && (action == GLFW_REPEAT || action == GLFW_PRESS)) {
        if (useHeli) useHeli = 0;
        else useHeli = 1;
    }
}


void drawModel(Render renPara) {
    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    if (renPara.modelName == "plane") glBindTexture(GL_TEXTURE_2D, airplaneTexture);
    else if (renPara.modelName == "earth") glBindTexture(GL_TEXTURE_2D, earthTexture);
    glUniformMatrix4fv(renPara.modelLoc, 1, GL_FALSE, glm::value_ptr(renPara.model));
    glUniformMatrix4fv(renPara.viewLoc, 1, GL_FALSE, glm::value_ptr(renPara.view));
    glUniformMatrix4fv(renPara.projLoc, 1, GL_FALSE, glm::value_ptr(renPara.projection));
    glUniform1f(renPara.squeezLoc, glm::radians(squeezeFactor));
    glUniform3fv(renPara.rainbowLoc, 1, glm::value_ptr(renPara.color));
    glUniform1i(renPara.useRainbowColorLoc, renPara.useRainbow);
    glUniform1i(renPara.useSqueezeLoc, renPara.useSqueeze);
    glUniform1i(renPara.useHeliLoc, renPara.useHeli);
    glUniform1f(renPara.textureLoc, 0);
    if (renPara.modelName == "plane") {
        glBindVertexArray(airplaneVAO);
        glDrawArrays(GL_TRIANGLES, 0, airplaneObject->positions.size());
    }
    else if (renPara.modelName == "earth") {
        glBindVertexArray(earthVAO);
        glDrawArrays(GL_TRIANGLES, 0, earthObject->positions.size());
    }
    else {  // - draw cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, cubeObject->positions.size());
    }
    glBindVertexArray(0);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void init() {
#if defined(__linux__) || defined(__APPLE__)
    string dirShader = "../../src/shaders/";
    string dirAsset = "../../src/asset/obj/";
    string dirTexture = "../../src/asset/texture/";
#else
    string dirShader = "..\\..\\src\\shaders\\";
    string dirAsset = "..\\..\\src\\asset\\obj\\";
    string dirTexture = "..\\..\\src\\asset\\texture\\";
#endif

    // Object
    airplaneObject = new Object(dirAsset + "airplane.obj");
    earthObject = new Object(dirAsset + "earth.obj");
    cubeObject = new Object(dirAsset + "cube.obj");

    // Shader
    vertexShader = createShader(dirShader + "vertexShader.vert", "vert");
    fragmentShader = createShader(dirShader + "fragmentShader.frag", "frag");
    shaderProgram = createProgram(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // Texture
    airplaneTexture = loadTexture(dirTexture + "airplane.jpg");
    earthTexture = loadTexture(dirTexture + "earth.jpg");

    // VAO, VBO
    airplaneVAO = modelVAO(*airplaneObject);
    earthVAO = modelVAO(*earthObject);
    cubeVAO = modelVAO(*cubeObject);
}
