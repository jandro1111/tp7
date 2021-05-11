#include "gui.h"

using namespace std;

ALLEGRO_DISPLAY* displaygui;
ALLEGRO_EVENT_QUEUE* eventQueue;

int gui_init(void){  //inicializacion de allegro de los elementos que se utilizaran
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
	
	if ((displaygui = al_create_display(500, 300)) == NULL) {
		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_shutdown_primitives_addon();
		cout << "failed to create display!" << endl;
		return false;
	}
	eventQueue = al_create_event_queue();
	if (!eventQueue) {
		al_destroy_display(displaygui);
		al_uninstall_keyboard();
		al_uninstall_mouse();
		al_shutdown_primitives_addon();
		cout << "failed to load event queue!" << endl;
		return false;
	}
	al_set_window_title(displaygui, "Display GUI");
	al_register_event_source(eventQueue, al_get_display_event_source(displaygui));  //se registran los eventos del display, mouse y teclado
    al_register_event_source(eventQueue, al_get_keyboard_event_source());
    al_register_event_source(eventQueue, al_get_mouse_event_source());

	IMGUI_CHECKVERSION();			// Control de version de Dear ImGui
	ImGui::CreateContext();
	ImGui_ImplAllegro5_Init(displaygui); // Inicializa Dear ImGui
	ImGuiIO& io = ImGui::GetIO();

	ImGui::StyleColorsLight();
	al_flip_display();
	return true;
}

void gui_uninst()  //destruye los elementos inicializados anteriormente
{
	ImGui_ImplAllegro5_Shutdown();
	ImGui::DestroyContext();
	al_destroy_event_queue(eventQueue);
	al_destroy_display(displaygui);
	al_shutdown_primitives_addon();
	al_uninstall_mouse();
	al_uninstall_keyboard();
}

void gui_input(int& flag, int& canttwits, string& autor)  //crea la primera gui que se muestra
{
	al_set_target_backbuffer(displaygui);
	autor.clear();
	char tempstring[15] = {};
	while (flag != GO)
	{
		ALLEGRO_EVENT ev;
		while (al_get_next_event(eventQueue, &ev))
		{
			ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese
		}

		// Inicio el frame. 
		ImGui_ImplAllegro5_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("INPUTS");  //dibuja la ventana donde apareceran los botones y cuadros de texto
		if (flag == WRONG)
		{
			ImGui::Text("Username can't be empty");
		}
		if (ImGui::Button("GO")) {
			flag = GO;
		}
		if (autor == "")
		{
			ImGui::InputText("Username", tempstring, 15);
		}
		if (flag == GO)
		{
			autor = string(tempstring);
		}

		static int i0 = 0;
		ImGui::InputInt("Amount of tweets", &i0);
		canttwits = i0;
		ImGui::End();
		ImGui::Render();	

		al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));	//Va a quedar detras de las ventanas.
		ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()
		al_flip_display(); //DearImGui nunca hace al_flip_display()
		if (flag == WRONG)
		{
			al_rest(1);
			flag = DONO;
		}
	}
	flag = DONO;
}

void gui_searching(int& flag)  //crea la gui que se muestra mientras cargan los tweets
{
		al_set_target_backbuffer(displaygui);
		ALLEGRO_EVENT ev;
		while (al_get_next_event(eventQueue, &ev))
		{
			ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese
		}

		ImGui_ImplAllegro5_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("SEARCHING");
		if (ImGui::Button("Cancel")) {
			flag = EXIT;
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
		ImGui::Render();	

		al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));	//Va a quedar detras de las ventanas.
		ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()
		al_flip_display(); //DearImGui nunca hace al_flip_display()
}

void gui_showtw (int& flag, float& vel) //crea la gui que se muestra mientras se pasan los tweets
{
	al_set_target_backbuffer(displaygui);
	ALLEGRO_EVENT ev;
	while (al_get_next_event(eventQueue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

	}

	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("SHOWING TWEETS");
	if (ImGui::Button("Previous")) {
		flag = PREV;
	}
	if (ImGui::Button("Repeat")) {
		flag = REP;
	}
	if (ImGui::Button("Next")) {
		flag = NEXT;
	}
	if (ImGui::Button("Exit")) {
		flag = EXIT;
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
	ImGui::Render();	

	al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));	//Va a quedar detras de las ventanas.
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()
	al_flip_display(); //DearImGui nunca hace al_flip_display()
}

int gui_ending(int& flag) //crea la gui que se muestra una vez que se pasaron todos los tweets
{
	int temp;
	al_set_target_backbuffer(displaygui);
	ALLEGRO_EVENT ev;
	while (al_get_next_event(eventQueue, &ev))
	{
		ImGui_ImplAllegro5_ProcessEvent(&ev);	// Mandar el evento a Dear ImGui para que lo procese

	}

	// Inicio el frame. Se realiza una vez por cada pantalla que dibujo.
	ImGui_ImplAllegro5_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("END");
	ImGui::Text("Want to download more tweets?");
	static int i0 = 0;
	ImGui::InputInt("Cantidad de tweets", &i0);
	temp = i0;
	if (ImGui::Button("GO")) {
		flag = GO;
	}
	if (ImGui::Button("Exit")) {
		flag = EXIT;
	}
	ImGui::End();
	ImGui::Render();	

	al_clear_to_color(al_map_rgba_f(1, 1, 0.8, 1));	//Va a quedar detras de las ventanas.
	ImGui_ImplAllegro5_RenderDrawData(ImGui::GetDrawData());	//Dibuja las ventanas, pero no hace al_flip_display()
	al_flip_display(); //DearImGui nunca hace al_flip_display()
	return temp;
}