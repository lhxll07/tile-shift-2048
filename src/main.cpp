#include <iostream>
#include <string>
#include <Windows.h>
#include <conio.h>
#include <vector>
#include <random>
#include <stdlib.h>
#include <winbase.h>
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

using namespace std;

bool restart_flag = false;

//定义用于实现2048功能的类
class board{
    private:
    int length;
    int width;
    int times;
    int marks;
    int moved;
    vector<vector<int>> b;

    public:
    board(int l, int w): length{l}, width{w}, times{0}, marks{0}, moved{0}, b(l,vector<int>(w,0)) {}

    bool is_blank(int m){
        return m==0;
    }

    bool is_full(){
        for(auto y : b){
            for(auto x : y){
                if(is_blank(x)){
                    return false;
                }
            }
        }
        return true;
    }

    bool random_init(){
        if(is_full()){
            return false;
        }
        vector<int> wid;
        vector<int> len;
        for(int y = 0;y < length;y++){
            for(int x = 0;x < width;x++){
                if(is_blank(b[y][x])){
                    len.push_back(y);
                    wid.push_back(x);
                }
            }
        }
        random_device rdm;
        mt19937 engine(rdm());
        uniform_int_distribution<int> dist(0, wid.size()-1);
        int tar = dist(engine);
        b[len[tar]][wid[tar]] = 2;
        return true;
    }

    void process(int dire){
        moved = 0;
        switch(dire){
            case 72:
            for(int y = 1;y < length;y++){
                for(int x = 0;x < width;x++){
                    process_in(x, y, dire);
                }
            }; break;
            case 80:
            for(int y = length-2;y >=0;y--){
                for(int x = 0;x < width;x++){
                    process_in(x, y, dire);
                }
            }; break;
            case 77:
            for(int x = width-2;x >= 0;x--){
                for(int y = 0;y < length;y++){
                    process_in(x, y, dire);
                }
            }; break;
            case 75:
            for(int x = 1;x < width;x++){
                for(int y = 0;y < length;y++){
                    process_in(x, y, dire);
                }
            }; break;
        }
    }
    void process_in(int x, int y, int dire){
        if(is_blank(b[y][x])){
            return;
        }else if(is_bondary(x, y, dire)){
            return;
        }else if(is_blocked(x, y, dire)){
            return;
        }else{
            int tempx = x,tempy = y;
            while(!is_bondary(tempx, tempy, dire) && !is_blocked(tempx, tempy, dire)){
                if(if_merge(tempx, tempy, dire)){
                    merge(tempx, tempy, dire);
                    break;
                }
                move(tempx, tempy, dire);
                switch(dire){
                    case 72:tempy--;break;
                    case 80:tempy++;break;
                    case 77:tempx++;break;
                    case 75:tempx--;break;
                }
            }
            return;
        }
    }
    bool if_merge(const int x, const int y, const int direction){
        switch(direction){
            case 72:if(b[y][x] == b[y-1][x]){ return true; } break;//方向键上
            case 80:if(b[y][x] == b[y+1][x]){ return true; } break;//方向键下
            case 77:if(b[y][x] == b[y][x+1]){ return true; } break;//方向键右
            case 75:if(b[y][x] == b[y][x-1]){ return true; } break;//方向键左
        }
        return false;
    }

    void merge(const int x, const int y, const int direction){
        switch(direction){
            case 72:b[y-1][x] += b[y][x]; b[y][x] = 0;break;//方向键上
            case 80:b[y+1][x] += b[y][x]; b[y][x] = 0;break;//方向键下
            case 77:b[y][x+1] += b[y][x]; b[y][x] = 0;break;//方向键右
            case 75:b[y][x-1] += b[y][x]; b[y][x] = 0;break;//方向键左
        }
        moved = 1;
        marks += 5;
    }

    bool is_bondary(const int x, const int y, const int direction){
        switch(direction){
            case 72:if(y <= 0){ return true; }break;//方向键上
            case 80:if(y >= length-1){ return true; }break;//方向键下
            case 77:if(x >= width-1){ return true; }break;//方向键右
            case 75:if(x <= 0){ return true; }break;//方向键左
        }
        return false;
    }

    bool is_blocked(const int x, const int y, const int direction){
        switch(direction){
            case 72:return b[y-1][x] && b[y-1][x] != b[y][x];break;//方向键上
            case 80:return b[y+1][x] && b[y+1][x] != b[y][x];break;//方向键下
            case 77:return b[y][x+1] && b[y][x+1] != b[y][x];break;//方向键右
            case 75:return b[y][x-1] && b[y][x-1] != b[y][x];break;//方向键左
        }
        return false;
    }

    void move(const int x, const int y, const int direction){
        switch(direction){
            case 72:b[y-1][x] = b[y][x]; b[y][x] = 0;break;//方向键上
            case 80:b[y+1][x] = b[y][x]; b[y][x] = 0;break;//方向键下
            case 77:b[y][x+1] = b[y][x]; b[y][x] = 0;break;//方向键右
            case 75:b[y][x-1] = b[y][x]; b[y][x] = 0;break;//方向键左
        }
        moved = 1;
    }

    void flash(int dire){
        if(times ==0){
            random_init();
            times++;
        }else{
            process(dire);
            if(moved ==1){
                random_device rdm;
                mt19937 engine(rdm());
                uniform_int_distribution<int> dist(1, 2);
                int n = dist(engine);
                for(int i = 1;i<=n;i++){
                    random_init();
                }
            }
        }
    }

    bool is_win(){
        for(auto& y : b){
            for(auto& x : y){
                if(x==2048){
                    return true;
                }
            }
        }
        return false;
    }

    bool is_failed(){
        if(times==0){
            return false;
        }
        board copy_u = *this, copy_d = *this, copy_l = *this, copy_r = *this;
        copy_u.process(72);
        copy_d.process(80);
        copy_r.process(77);
        copy_l.process(75);
        if(copy_u.b!=b){
            return false;
        }else if(copy_d.b!=b){
            return false;
        }else if(copy_r.b!=b){
            return false;
        }else if(copy_l.b!=b){
            return false;
        }else{
            return true;
        }
    }

    void Display(bool is_win, bool is_lost, ImFont* button_font){
        if(is_win){
            ImGui::Text("you win");
            ImGui::NewLine();
            if(ImGui::Button("restart")){
                restart_flag = true;
            }
        }else if(is_lost){
            ImGui::Text("you lost");
            ImGui::NewLine();
            if(ImGui::Button("restart")){
                restart_flag = true;
            }
        }
        ImGui::Text("Scores : %d", marks);
        for(int y = 0;y < length;y++){
            for(int x = 0;x<width;x++){
                ImGui::PushID(y * width + x);
                ImGui::PushFont(button_font); // 传入main中加载的字体指针
                ImGui::BeginDisabled(); 
                string num = (b[y][x] == 0) ? " " : to_string(b[y][x]);
                ImGui::Button(num.c_str(), ImVec2(100,100));
                ImGui::EndDisabled();
                ImGui::PopFont(); // 恢复原有字体，不影响后续UI
                ImGui::PopID();
                if(x<width-1){ ImGui::SameLine(); }
            }
            ImGui::Dummy(ImVec2(0,0));
        }
    }

    int show_marks(){
        return marks;
    }
};

int main(){
    int length = 4,width = 4;
    board game(length,width);

    glfwInit();
    //初始化GLFW版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    //初始化GLFW窗口
    glfwWindowHint(GLFW_VISIBLE, TRUE);
    GLFWwindow* window = glfwCreateWindow(440, 700, "Tile Shift 2048", NULL, NULL);
    glfwMakeContextCurrent(window);
    
    //初始化ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    //自定义字体
    ImFontConfig font_cfg;
    font_cfg.SizePixels = 40.0f; // 按钮文字字号（可改：24/32/40等）
    ImFont* button_font;
    button_font = io.Fonts->AddFontDefault(&font_cfg);

    //设置平台，ImGui后端(OpenGL窗口)
    ImGui_ImplGlfw_InitForOpenGL(window, true);        // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init("#version 330");
    //设置键盘冷却时间
    double last_key_time = 0.0;
    const double key_cooldown = 0.15;
    // (Your code calls glfwPollEvents())
    // ...
    // Start the Dear ImGui frame
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();
        double current_time = glfwGetTime();

        // 主循环内（ImGui::NewFrame() 之前）
        ImGuiIO& io = ImGui::GetIO();
        if(restart_flag){
            game = board(length,width);
            restart_flag = false;
        }
        bool is_win = game.is_win();
        bool is_failed = game.is_failed();
        bool is_game_over = is_win || is_failed;

        if(current_time - last_key_time > key_cooldown){
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
                game.flash(72);
                last_key_time = current_time;
            } else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
                game.flash(80);
                last_key_time = current_time;
            } else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                game.flash(75);
                last_key_time = current_time;
            } else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                game.flash(77);
                last_key_time = current_time;
            }
        }
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h); // 获取 GLFW 窗口绘制区大小
        ImGui::SetNextWindowPos(ImVec2(0, 0)); // 位置：GLFW 窗口左上角
        ImGui::SetNextWindowSize(ImVec2(display_w, display_h)); // 大小：和 GLFW 窗口一致
        ImGui::Begin("Tile Shift 2048", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

        game.Display(is_win, is_failed, button_font);
        
        ImGui::End(); // 关闭窗口

        // Rendering
        // (Your code clears your framebuffer, renders your other stuff etc.)
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
    
    //清理资源
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
