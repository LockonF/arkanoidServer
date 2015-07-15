# Arkanoid Server

Esta implementación del servidor lee, une y responde con broadcast a los mensajes de las conecciones que le envíen mensaje.

Los archivos deben de estar en la misma carpeta, pero XCode los separó.

##Clases

Las clases a utilizar son las siguientes

###Mensaje

Solamente contiene un encabezado y un cuerpo. En encabezado contiene el número de caracteres del mensaje y el cuerpo lo demás. Se puede modificar para que se acepte un número máximo de caracteres. Por el momento tiene un valor de 3000. Esta es nuestra unidad de transmisión.

###TCPServer

Esta clase solamente implementa la aceptación del socket y crea un game_room como apuntador compartido.

Nota: El archivo TCPServer.h tiene otras clases, las cuales están comentadas.


##Cosas Pendientes

..* En la clase game_room, se puede añadir la estructura tablero, para ir actualizandola con el tablero al momento de deserializar.
..* En el método do_read_body() de la clase game_Session se puede hacer la implementación de la deserialización. Para esto se debería de agregar un método en la clase game_room para manipular la estructura.
..* Probar que todo lo demás funcione correctamente