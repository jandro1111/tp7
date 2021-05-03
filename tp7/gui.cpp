#include "gui.h"

using namespace std;

int gui_init(int flag){
	init_allegro();
	ALLEGRO_DISPLAY* display = al_create_display(1000, 300);
	IMGUI_CHECKVERSION();			// Control de version de Dear ImGui
	ImGui::CreateContext(); 
	ImGui_ImplAllegro5_Init(display); // Inicializa Dear ImGui

	ImGui::StyleColorsLight();
	bool running = true;			// true hasta que se cierre el display.
	bool keep_open = true;			// true hasta que se cierre la ventana a la que se asigna.

	ImGui::Begin("INIT");
		if (ImGui::Button("GO")){
			flag = GO;
		}
		ImGui::End();
		ImGui::InputText("usuario", buf, 256);
		ImGui::SliderFloat("cant. de twits", &f, 0.0f, 1.0f); 

	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(queue);
	al_destroy_display(display);
	al_shutdown_primitives_addon();
	return 0;
}

int gui_searching(int flag){

	return 0;
}

int gui_showtw (int flag){

	return 0;
}