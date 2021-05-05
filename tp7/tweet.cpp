#include "tweet.h"
using json = nlohmann::json;

int dostuff(string autor, std::list<twits>& tweet) {
	json j;                    //Variable donde vamos a guardar lo que devuelva Twitter

	// Vamos a utilizar la librer�a CURL ya que debemos conectarons a un servidor HTTPS
	// (HTTP seguro) el cual requeire un protocolo especial de encriptaci�n
	// m�s complejo que el plain HTTP que utilizamos en el TP de Networking.

	CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
	CURLM* multiHandle;			//Variable donde vamos a atachear los easy handles
	CURLcode res;
	std::string readString, token;
	std::list<std::string> date;
	// Query es la direcci�n de Twitter que vamos a consultar. vamos a bajar los &count twits de screen_name en formato JSON.
	std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";//este numero es la cantidad de twetts  &count=0
	query += autor;
	// Las dos constantes de abajo son el API_Key y API_SecretKey que necesita Twitter para realizar la autenticaci�n de nuestra App
	// y por lo tanto permitirnos el acceso a sus servidores. Son parte de la estrategia de autenticaci�n Oauth2.
	//Ambas se obtienen de la app que creen en la pagina de Twitter
	std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
	std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";


	std::list<std::string> names;

	/************************************************************************************
	*                      Get Bearer Token from the Twitter API						*
	************************************************************************************/

	// Primero realizamos la inicializaci�n de curl para obtener el token de Twitter el cual vamos
	// a utilizar luego para realizar la autenticaci�n por Oauth2.
	curl = curl_easy_init();
	if (curl)
	{
		//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
		curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

		// Si la p�gina nos redirije a alg�n lado, le decimos a curl que siga dicha redirecci�n.
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticaci�n por HTTP en modo b�sico.
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
		curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

		//Preparamos el password para la autenticaci�n
		std::string userPwd = API_key + ":" + API_SecretKey;
		//Se lo seteamos a CURL.
		curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

		struct curl_slist* list = NULL;
		//Pueden setear el Header con la linea de abajo, pero necesitan codificar las claves en Base64
		//list = curl_slist_append(list, "Authorization: Basic YlB5alk1bWRMR2V4TlhPTHhSUjd3MUVjUzpkR2liU3FIcURrektQMUtPbzFJTjRBd21tZGI1Tnl5ZjBFQTZkTDBLWlpmZDE0ZnhQQw==");

		//Le decimos a CURL que vamos a mandar URLs codificadas y adem�s en formato UTF8.
		list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Le decimos a CURL que trabaje con credentials.
		std::string data = "grant_type=client_credentials";
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

		//Le decimos a curl que cuando haya que escribir llame a myCallback
		//y que use al string readString como user data.
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		// Perform the request, res will get the return code
		// Con lo de abajo le decimos a curl que intente conectarse a la p�gina (recordemos que la idea es
		// obtener la autenticaci�n as� luego Twitter nos deja conectarnos a cualquier usuario p�blico)
		//Recordar que easy_perform es bloqueante, en este caso es correcto asi al ser inicializacion y no tardar mucho tiempo.
		res = curl_easy_perform(curl);
		//

		//ACA imgui interrupcion

		//
		// Nos fijamos si hubo alg�n error
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return 0;
		}

		// Si no hubo errores hago un clean up antes de realizar un nuevo query.
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//Abajo busco el token en el JSON para luego acceder a los twits.
		j = json::parse(readString);

		//Se encierra el parseo en un bloque try-catch porque la libreria maneja errores por excepciones.
		//Pueden tambien parsear usando iteradores o la forma que quieras, buscar en la documentacion las diferentes formas.
		try
		{
			//Tratamos de acceder al campo acces_token del JSON
			std::string aux = j["access_token"];
			token = aux;
			std::cout << "Bearer Token get from Twitter API: \n" << token << std::endl;
		}
		catch (std::exception& e)
		{
			//Si hubo algun error, se muestra el error que devuelve la libreria
			std::cerr << e.what() << std::endl;
			return 0;
		}
	}
	else
	{
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
		return 0;
	}

	//Una vez obtenido el Token ahora voy a buscar los Twits

	/************************************************************************************
	*                        Get Tweets from the Twitter API							*
	************************************************************************************/

	// Nuevamente reinicializo todo curl.
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist* list = NULL;
		std::string aux = "Authorization: Bearer ";
		aux = aux + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning)
		{
			//Debemos hacer polling de la transferencia hasta que haya terminado
			curl_multi_perform(multiHandle, &stillRunning);

			//Mientras tanto podemos hacer otras cosas
		}

		//Checkeamos errores
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return 0;
		}

		//Siempre realizamos el cleanup al final
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//con toda la informacion de los tweets que le pedimos
		j = json::parse(readString);
		try
		{
			//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
			for (auto element : j) {
				names.push_back(element["created_at"]);
				names.push_back(element["text"]);
			}
			std::cout << "Tweets retrieved from Twitter account: " << std::endl;
			printNames(names,tweet,autor);
		}
		catch (std::exception& e)
		{
			//Muestro si hubo un error de la libreria
			std::cerr << e.what() << std::endl;
		}
	}
	else
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
	return 0;
}
	int dostuff(string autor,int cant, std::list<twits>& tweet) {
		json j;                    //Variable donde vamos a guardar lo que devuelva Twitter

		// Vamos a utilizar la librer�a CURL ya que debemos conectarons a un servidor HTTPS
		// (HTTP seguro) el cual requeire un protocolo especial de encriptaci�n
		// m�s complejo que el plain HTTP que utilizamos en el TP de Networking.

		CURL* curl;					//Variable donde vamos a guardar las configuraciones de una transferencia
		CURLM* multiHandle;			//Variable donde vamos a atachear los easy handles
		CURLcode res;
		std::string readString, token;
		// Query es la direcci�n de Twitter que vamos a consultar. vamos a bajar los &count twits de screen_name en formato JSON.
		std::string query = "https://api.twitter.com/1.1/statuses/user_timeline.json?screen_name=";//este numero es la cantidad de twetts  &count=0
		//conectar con imgui
		query += autor;
		query += "&count=";
		query += std::to_string(cant);
		// Las dos constantes de abajo son el API_Key y API_SecretKey que necesita Twitter para realizar la autenticaci�n de nuestra App
		// y por lo tanto permitirnos el acceso a sus servidores. Son parte de la estrategia de autenticaci�n Oauth2.
		//Ambas se obtienen de la app que creen en la pagina de Twitter
		std::string API_key = "HCB39Q15wIoH61KIkY5faRDf6";
		std::string API_SecretKey = "7s8uvgQnJqjJDqA6JsLIFp90FcOaoR5Ic41LWyHOic0Ht3SRJ6";


		std::list<std::string> names;

		/************************************************************************************
		*                      Get Bearer Token from the Twitter API						*
		************************************************************************************/

		// Primero realizamos la inicializaci�n de curl para obtener el token de Twitter el cual vamos
		// a utilizar luego para realizar la autenticaci�n por Oauth2.
		curl = curl_easy_init();
		if (curl)
		{
			//Seteamos primero la pagina donde nos vamos a conectar. Para buscar el token es siempre la misma
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.twitter.com/oauth2/token");

			// Si la p�gina nos redirije a alg�n lado, le decimos a curl que siga dicha redirecci�n.
			curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

			// Le decimos a CURL que trabaje tanto con HTTP como HTTPS, Autenticaci�n por HTTP en modo b�sico.
			curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);
			curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

			//Preparamos el password para la autenticaci�n
			std::string userPwd = API_key + ":" + API_SecretKey;
			//Se lo seteamos a CURL.
			curl_easy_setopt(curl, CURLOPT_USERPWD, userPwd.c_str());

			struct curl_slist* list = NULL;
			//Pueden setear el Header con la linea de abajo, pero necesitan codificar las claves en Base64
			//list = curl_slist_append(list, "Authorization: Basic YlB5alk1bWRMR2V4TlhPTHhSUjd3MUVjUzpkR2liU3FIcURrektQMUtPbzFJTjRBd21tZGI1Tnl5ZjBFQTZkTDBLWlpmZDE0ZnhQQw==");

			//Le decimos a CURL que vamos a mandar URLs codificadas y adem�s en formato UTF8.
			list = curl_slist_append(list, "Content-Type: application/x-www-form-urlencoded;charset=UTF-8");
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

			//Le decimos a CURL que trabaje con credentials.
			std::string data = "grant_type=client_credentials";
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

			//Le decimos a curl que cuando haya que escribir llame a myCallback
			//y que use al string readString como user data.
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

			// Perform the request, res will get the return code
			// Con lo de abajo le decimos a curl que intente conectarse a la p�gina (recordemos que la idea es
			// obtener la autenticaci�n as� luego Twitter nos deja conectarnos a cualquier usuario p�blico)
			//Recordar que easy_perform es bloqueante, en este caso es correcto asi al ser inicializacion y no tardar mucho tiempo.
			res = curl_easy_perform(curl);
			//

			//ACA imgui interrupcion

			//
			// Nos fijamos si hubo alg�n error
			if (res != CURLE_OK)
			{
				std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
				//Hacemos un clean up de curl antes de salir.
				curl_easy_cleanup(curl);
				return 0;
			}

			// Si no hubo errores hago un clean up antes de realizar un nuevo query.
			curl_easy_cleanup(curl);

			//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
			//Abajo busco el token en el JSON para luego acceder a los twits.
			j = json::parse(readString);

			//Se encierra el parseo en un bloque try-catch porque la libreria maneja errores por excepciones.
			//Pueden tambien parsear usando iteradores o la forma que quieras, buscar en la documentacion las diferentes formas.
			try
			{
				//Tratamos de acceder al campo acces_token del JSON
				std::string aux = j["access_token"];
				token = aux;
				std::cout << "Bearer Token get from Twitter API: \n" << token << std::endl;
			}
			catch (std::exception& e)
			{
				//Si hubo algun error, se muestra el error que devuelve la libreria
				std::cerr << e.what() << std::endl;
				return 0;
			}
		}
		else
		{
			std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
			return 0;
		}



	//Una vez obtenido el Token ahora voy a buscar los Twits

	/************************************************************************************
	*                        Get Tweets from the Twitter API							*
	************************************************************************************/

	// Nuevamente reinicializo todo curl.
	curl = curl_easy_init();
	multiHandle = curl_multi_init();
	readString = "";
	int stillRunning = 0;
	if ((curl != NULL) & (multiHandle != NULL))
	{
		//Attacheo el easy handle para manejar una coneccion no bloqueante.
		curl_multi_add_handle(multiHandle, curl);

		//Seteamos URL FOLLOWLOCATION y los protocolos a utilizar igual que antes.
		curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_PROTOCOLS, CURLPROTO_HTTP | CURLPROTO_HTTPS);

		//Construimos el Header de autenticacion como lo especifica la API
		//usando el token que obtuvimos antes
		struct curl_slist* list = NULL;
		std::string aux = "Authorization: Bearer ";
		aux = aux + token;
		list = curl_slist_append(list, aux.c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

		//Seteamos los callback igual que antes
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, myCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readString);

		//Realizamos ahora un perform no bloqueante
		curl_multi_perform(multiHandle, &stillRunning);
		while (stillRunning)
		{
			//Debemos hacer polling de la transferencia hasta que haya terminado
			curl_multi_perform(multiHandle, &stillRunning);

			//Mientras tanto podemos hacer otras cosas
		}

		//Checkeamos errores
		if (res != CURLE_OK)
		{
			std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
			//Hacemos un clean up de curl antes de salir.
			curl_easy_cleanup(curl);
			return 0;
		}

		//Siempre realizamos el cleanup al final
		curl_easy_cleanup(curl);

		//Si el request de CURL fue exitoso entonces twitter devuelve un JSON
		//con toda la informacion de los tweets que le pedimos
		j = json::parse(readString);
		try
		{
			//Al ser el JSON un arreglo de objetos JSON se busca el campo text para cada elemento
			for (auto element : j) {
				names.push_back(element["created_at"]);
				names.push_back(element["text"]);
			}
			std::cout << "Tweets retrieved from Twitter account: " << std::endl;
			printNames(names,tweet,autor);
		}
		catch (std::exception& e)
		{
			//Muestro si hubo un error de la libreria
			std::cerr << e.what() << std::endl;
		}
	}
	else
		std::cout << "Cannot download tweets. Unable to start cURL" << std::endl;
	return 0;
}

//Funcion auxiliar para imprimir los tweets en pantalla una vez parseados
void printNames(std::list<std::string> names, std::list<twits>& tweet,string autor)//      modificar esto para lcd
{
	bool alternate = true;
	twits aux;
	string auxst="";
	for (auto c : names)//voy a tener primero fecha y despues texto, tengo que alternar
	{
		aux.author = autor;
		if (alternate == true) {//guardo la fecha
			//int extended = (int)c.find("+0000");
			//c = c.substr(0, extended);
			auxst = dateadj(c);
			aux.date = auxst;
			alternate = false;
		}
		else {//guardo el texto
			aux.body = autor;
			aux.body += ": - ";
			int extended = (int)c.find("https");
			c = c.substr(0, extended);
			aux.body += c;
			aux.body += " -";
			tweet.push_back(aux);
			alternate = true;
		}	
		//Eliminamos el URL al final para mostrar
		std::cout << c << std::endl;
		std::cout << "-----------------------------------------" << std::endl;
	}
}

//Concatena lo recibido en content a s
static size_t myCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	char* data = (char*)contents;
	//fprintf(stdout, "%s",data);
	std::string* s = (std::string*)userp;
	s->append(data, realsize);
	return realsize;						//recordar siempre devolver realsize
}

//ajusta la fecha
std::string dateadj(std::string date) {// Tue May 04 16:30:27 +0000 2021 en vez de este DD/MM/AA � hh:mm
	char buffer[100];
	char aux[16];
	std::string aux2="";
	for (int i = 0; i < 100; ++i) {
		buffer[i] = 0;
	}
	for (int i = 0; i < 16; ++i) {
		aux[i] = 0;
	}
	strcpy(buffer, date.c_str());
	aux[0] = buffer[8];
	aux[1] = buffer[9];
	aux[2] = '/';
	aux2 += buffer[4];
	aux2 += buffer[5];
	aux2 += buffer[6];
	if (aux2=="Jan") {
		aux[3] = '0';
		aux[4] = '1';
	}
	else {
		if (aux2 == "Jan") {
			aux[3] = '0';
			aux[4] = '1';
		}
		else {
			if (aux2 == "Feb") {
				aux[3] = '0';
				aux[4] = '2';
			}
			else {
				if (aux2 == "Mar") {
					aux[3] = '0';
					aux[4] = '3';
				}
				else {
					if (aux2 == "Apr") {
						aux[3] = '0';
						aux[4] = '4';
					}
					else {
						if (aux2 == "May") {
							aux[3] = '0';
							aux[4] = '5';
						}
						else {
							if (aux2 == "Jun") {
								aux[3] = '0';
								aux[4] = '6';
							}
							else {
								if (aux2 == "Jul") {
									aux[3] = '0';
									aux[4] = '7';
								}
								else {
									if (aux2 == "Aug") {
										aux[3] = '0';
										aux[4] = '8';
									}
									else {
										if (aux2 == "Sep") {
											aux[3] = '0';
											aux[4] = '9';
										}
										else {
											if (aux2 == "Oct") {
												aux[3] = '1';
												aux[4] = '0';
											}
											else {
												if (aux2 == "Nov") {
													aux[3] = '1';
													aux[4] = '1';
												}
												else {
													if (aux2 == "Dec") {
														aux[3] = '1';
														aux[4] = '2';
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	aux[5] = '/';
	aux[6] = buffer[28];
	aux[7] = buffer[29];
	aux[8] = ' ';
	aux[9] = '-';
	aux[10] = ' ';
	for (int i = 11; i < 16; ++i) {
		aux[i] = buffer[i];
	}
	date = aux;
	date.resize(16);
	date.shrink_to_fit();
	std::cout << date << std::endl;
	return date;
}