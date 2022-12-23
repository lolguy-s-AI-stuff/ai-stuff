#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shader.h>
#include <AI.h>


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
        int v[3]{0,0,0};
        unsigned char w[3]{0,0,0};
        unsigned char* out = &w[1];

        int weight[3][4][4]{
        {{255, 0, 0, 0}, {255, 0, 0, 0}, {255, 0, 0, 0}, {0, 0, 0, 0}},
        {{255, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
        {{0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
        };
        int bias[3][4]{
        	{0, 0, 0, 0},
            {0, 0, 0, 0},
            {0, 0, 0, 0}
        };
        unsigned char traindata[3][4]{
            {255,0,0,255},
            {0,255,0,32},
            {0,0,255,8}
        };
        AI ai;
        while(!glfwWindowShouldClose(window)){
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();



            ImGui::Separator();
            ImGui::SliderInt4("Column 1 ",weight[0][0],0,255);
            ImGui::SliderInt4("Column 1b",weight[0][1],0,255);
            ImGui::SliderInt4("Column 1c",weight[0][2],0,255);
            ImGui::SliderInt4("Column 1d",weight[0][3],0,255);
            ImGui::Separator();
            ImGui::SliderInt4("Column 2 ",weight[1][0],0,255);
            ImGui::SliderInt4("Column 2b",weight[1][1],0,255);
            ImGui::SliderInt4("Column 2c",weight[1][2],0,255);
            ImGui::SliderInt4("Column 2d",weight[1][3],0,255);
            ImGui::Separator();
            ImGui::Separator();
            ImGui::SliderInt4("Column 1 of biases",bias[0],0,255);
            ImGui::Separator();
            ImGui::SliderInt4("Column 2 of biases",bias[1],0,255);
            ImGui::Separator();
            ImGui::SliderInt3("Inputs",v,0,255);
            ImGui::LabelText("","Output: %i",out[0]);

            if(ai.training){
                ai.train(3,(unsigned char**)traindata);
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        for (size_t k = 0; k < 4; k++)
                        {
                            weight[i][j][k] = ai.weights[i][j][k];
                        }
                        
                    }
                    
                }
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                            bias[i][j] = ai.biases[i][j];
                        
                    }
                    
                }
            if(ImGui::Button("Stop Training")){
                ai.training = false;
                
            }
            }else{
            if(ImGui::Button("Train")){
                ai.training = true;
            }
            }

            if(ImGui::Button("Run")){
                for (size_t i = 0; i < 3; i++)
                {
                   w[i] = (unsigned char) v[i];
                }
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        for (size_t k = 0; k < 4; k++)
                        {
                            ai.weights[i][j][k] = (unsigned char)weight[i][j][k];
                        }
                        
                    }
                    
                }

                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                            ai.biases[i][j] = bias[i][j];
                        
                    }
                    
                }
               out = ai.run(w);
            }
            if(ImGui::Button("Save")){
                                for (size_t i = 0; i < 3; i++)
                {
                   w[i] = (unsigned char) v[i];
                }
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        for (size_t k = 0; k < 4; k++)
                        {
                            ai.weights[i][j][k] = (unsigned char)weight[i][j][k];
                        }
                        
                    }
                    
                }

                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                            ai.biases[i][j] = bias[i][j];
                        
                    }
                    
                }   
                FILE* file = fopen("./ai.bin","w");
                ai.save(file);
                fclose(file);

            }
            if(ImGui::Button("Load")){
                FILE* file = fopen("./ai.bin","r");
                ai.load(file);
                fclose(file);
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                        for (size_t k = 0; k < 4; k++)
                        {
                            weight[i][j][k] = ai.weights[i][j][k];
                        }
                        
                    }
                    
                }
                for (size_t i = 0; i < 2; i++)
                {
                    for (size_t j = 0; j < 4; j++)
                    {
                            bias[i][j] = ai.biases[i][j];
                        
                    }
                    
                }
            }
         
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


