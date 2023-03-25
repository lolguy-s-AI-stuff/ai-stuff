#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <stdlib.h>
#include <shader.h>
#include <commons.h>


GLFWwindow* InitWindow()
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Open a window and create its OpenGL context
    GLFWwindow* window = glfwCreateWindow( 960, 540, "some random tools", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window.\n" );
        glfwTerminate();
        return nullptr;

    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{

		 std::cout << "Failed to initialize GLAD.\n";
         glfwTerminate();
         return nullptr;
	}

    std::cout << "Using openGL Version: " << glGetString(GL_VERSION) << std::endl;

    return window;
}

int main()
{
    GLFWwindow* window = InitWindow();
    if (!window)
        return -1;
        
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");
        ImGui::StyleColorsDark();
        //create varibles
        int num_layers = 3;
        int activation_function = 0;
        int* topology;
        topology = (int*)malloc(sizeof(int)*num_layers);
        for (size_t i = 0; i < num_layers; i++)
        {
            topology[i] = 3;
        }
        
        while(!glfwWindowShouldClose(window)){
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
        
            ImGui::NewFrame();


            ImGui::Begin("Multilayer Perceptron");
            //count and display the number of parameters
            int num_params = 0;
            for (size_t i = 0; i < num_layers; i++)
            {
                num_params += topology[i];
            }
            ImGui::Text("Number of Parameters: %d", num_params);
            ImGui::Separator();

            if(ImGui::InputInt("Number of Layers", &num_layers, 1)){
                if(num_layers < 2){
                    num_layers = 2;
                }
                //allocate a clear piece of memory for the new topology
                int* _topology = (int*)malloc(sizeof(int)*num_layers);
                memset(_topology, 0, sizeof(int)*num_layers);

                memcpy(_topology, topology, sizeof(int)*num_layers);
                free(topology);
                topology = _topology;
            }
            ImGui::Text("Activation Function");
            ImGui::RadioButton("Sigmoid", &activation_function, 0);
            ImGui::RadioButton("Tanh", &activation_function, 1);
            ImGui::RadioButton("ReLU", &activation_function, 2);
            ImGui::RadioButton("Leaky ReLU", &activation_function, 3);
            ImGui::RadioButton("Softmax", &activation_function, 4);
            ImGui::Separator();

            //custom element for setting up the number of neurons in each layer
            for (int i = 1; i <= num_layers; i++)
            {
                // input layer
                if (i == 1)
                {
                    ImGui::InputInt("Input Layer", &topology[i-1], 1);
                    ImGui::Separator();
                    continue;
                }
                //output layer
                else if (i == num_layers)
                {
                    ImGui::InputInt("Output Layer", &topology[i-1], 1);
                    ImGui::Separator();
                    continue;
                }

                // hidden layers
                std::string LayerIDFormattedToText = "Layer " + std::to_string(i);
                ImGui::InputInt(LayerIDFormattedToText.c_str(), &topology[i-1], 1);
                ImGui::Separator();

            }
            

            ImGui::End();
         
            ImGui::Render();
            
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
            }
        

            // Swap buffers
            glfwSwapBuffers(window);
            glfwPollEvents();

        
        }
    

    // Close OpenGL window and terminate GLFW
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}


