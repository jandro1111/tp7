#include "gui.h"

using namespace std;

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* eventQueue;

int gui_init(void){
	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed to initialize keyboard!\n");
		return false;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "Failed to initialize mouse!\n");
		al_uninstall_keyboard();
		return false;
	}
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "failed to initialize primitives!\n");
		al_uninstall_keyboard();
		al_uninstall_mouse();
		return false;
	}
	
	if (!(display = al_create_display(1000, 300))) {
		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_shutdown_primitives_addon();
		cout << "failed to create display!" << endl;
		return false;
	}
	eventQueue = al_create_event_queue();
	if (!eventQueue) {
		al_destroy_display(display);
		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_shutdown_primitives_addon();
		cout << "failed to load event queue!" << endl;
		return false;
	}
	al_register_event_source(eventQueue, al_get_display_event_source(display));
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

	IMGUI_CHECKVERSION();			// Control de version de Dear ImGui
	ImGui::CreateContext();
	ImGui_ImplAllegro5_Init(display); // Inicializa Dear ImGui
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsLight();

	return 0;
}

void gui_uninst()
{
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(eventQueue);
	al_destroy_display(display);
	al_shutdown_primitives_addon();
	al_uninstall_mouse();
	al_uninstall_keyboard();
}

void gui_input(int& flag, int& canttwits, string& autor)
{
	bool keep_open = true;			// true hasta que se cierre la ventana a la que se asigna.

	if (keep_open)
	{
		ImGui::Begin("INIT", &keep_open);
		if (ImGui::Button("GO")) {
			flag = GO;
		}
		autor.clear();
		ImGui::InputText("usuario", (char*)autor.c_str(), 15);
		static int i0 = 20;
		ImGui::InputInt("cantidad de tweets", &i0);
		ImGui::End();
	}
	else 
	{
		flag = CLOSE;
	}
	return;
}

void gui_searching(int& flag){
	bool keep_open = true;			// true hasta que se cierre la ventana a la que se asigna.

	if (keep_open)
	{
		ImGui::Begin("SEARCHING", &keep_open);
		if (ImGui::Button("Cancel")) {
			flag = CANCEL;
		}
		if (ImGui::Button("Display1")) {
			flag = LCD1;
		}
		if (ImGui::Button("Display2")) {
			flag = LCD2;
		}		
		if (ImGui::Button("Display3")) {
			flag = LCD3;
		}
		ImGui::End();
	}
	else 
	{
		flag = CLOSE;
	}
	return;
}

void gui_showtw (int& flag, float& vel){
	bool keep_open = true;			// true hasta que se cierre la ventana a la que se asigna.

	if (keep_open)
	{
		ImGui::Begin("SHOWING TWEETS", &keep_open);
		if (ImGui::Button("Previous")) {
			flag = PREV;
		}
		if (ImGui::Button("Repeat")) {
			flag = REP;
		}
		if (ImGui::Button("Next")) {
			flag = NEXT;
		}
		if (ImGui::Button("Cancel")) {
			flag = CANCEL;
		}
		ImGui::SliderFloat("Velocidad", &vel, 0.0f, 1.0f);
		if (ImGui::Button("Display1")) {
			flag = LCD1;
		}
		if (ImGui::Button("Display2")) {
			flag = LCD2;
		}		
		if (ImGui::Button("Display3")) {
			flag = LCD3;
		}
		ImGui::End();
	}
	else 
	{
		flag = CLOSE;
	}
	return;
}