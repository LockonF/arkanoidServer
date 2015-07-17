# Arkanoid Server

Esta implementación del servidor lee, une y responde con broadcast a los mensajes de las conecciones que le envíen mensaje.

Los archivos deben de estar en la misma carpeta, pero XCode los separó.

##Notas

###Clase udp_server

Tiene una función asíncrona que hace que espere una cantidad n de tiempo para ejecutarse junto con la recepción. Esto nos ayuda a que podamos enviar la estructura n veces a los clientes registrados

###Pendientes

Faltan los controles de lógica del deserializador