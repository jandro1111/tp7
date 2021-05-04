#include "gui.h"

using namespace std;

ALLEGRO_DISPLAY* display;
ALLEGRO_EVENT_QUEUE* eventQueue;

int gui_init(int flag){
	if (!al_init()) {
		fprintf(stderr, "Failed to initialize allegro!\n");
		return false;
	}
	if (!al_install_keyboard()) {
		fprintf(stderr, "Failed to initialize keyboard!\n");
		return false;
	}
	if (!al_install_mouse()) {
		fprintf(stderr, "Failed to initialize mouse!\n");
		al_uninstall_keyboard();
		al_uninstall_mouse();
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
	bool running = true;			// true hasta que se cierre el display.
	bool keep_open = true;			// true hasta que se cierre la ventana a la que se asigna.

	ImGui::Begin("INIT");
		if (ImGui::Button("GO")){
			flag = GO;
		}
		for (int i = 0; i < 16; i ++){
			username[i] = '\0';
		}
		ImGui::InputText("usuario", username, IM_ARRAYSIZE(username));
		static int i0 = 20;
        ImGui::InputInt("cantidad de tweets", &i0);
		ImGui::End();

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

int gui_searching(int flag){

	return 0;
}

int gui_showtw (int flag){

	return 0;
}