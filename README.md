# Virtual-library-data-base
#Copyright Stan Andreea-Cristina 323CA

Vreau sa incep prin a sustine faptul ca baza acestei teme a fost constituita de
rezolvarea oficiala a laboratorului 10 postata pe ocw. Astfel, functia 
compute_delete_request este realizata dupa complete_get_request, modificand doar
tipul metodei in DELETE. De asemenea, pentru parsarea json m-am folosit de sugestia
din cadrul enuntului temei si am descarcat de pe github-ul pus ca link fisierele
parson.c si parson.h destinate C si am folosit cateva din functiile existente acolo.

main.c - Aici are loc toata logica parsarii de la STDIN a comenzilor ce trebuie
        executate in cadrul programului. Pentru a retine token-ul si cookie-ul
        am apelat la un vector de string-uri alocat dinamic, metoda preluata din
        rezolvarea laboratorului pentru a nu fi nevoie de a modifica declaratiile
        functiilor din request.c spre exemplu. In functie de ce comanda trebuia
        executata, am apelat cateva functii ajutatoare care verifica, spre exemplu,
        daca valoarea introdusa de la tastatura este numar sau nu.
        La introducerea comenzii exit am avut grija sa eliberez memoria folosita
        evitand memory leak-urile.

commands.c - Aici se afla toate cererile trimise catre server si raspunsurile
            primite de la acesta.

        connection - Pentru a evita codul duplicat, am ales sa efectuez register
                    si login in aceeasi functie, intrucat difera doar url-ul din
                    mesaj.
                    -Pentru a crea corpul mesajului(format din username si parola)
                    m-am folosit atat de functiile din parson.c, cat si de un vector
                    de stringuri unde am memorat json-ul.
                    -Dupa ce am trimis cererea catre server, i-am memorat raspunsul
                    si am identificat tipul sau (eroare sau nu) si in cazul in care
                    era o eroare am apelat functia care parseaza intreg raspunsul si
                    salveaza linia cu eroarea din raspuns.
                    -Daca era o comanda de tip login si raspunsul nu reprezenta o
                    eroare, am memorat cookie-ul din raspuns si l-am returnat.

        enter - Reprezinta logica pentru enter_library, ca in toate functiile, creez
                mesajul pentru server, il trimit si memorez raspunsul, apoi apelez o
                functie ce-mi returneaza token-ul ce ma va ajuta sa accesez si sa
                actualizez date din librarie.

        add_book - Reprezinta logica pentru adaugarea unei carti in librarie.
                -Incep prin a-mi crea corpul pentru server ce v-a fi reprezentat tot de
                un json realizat la fel ca si in connection.
                -Si aici verific daca raspunsul serverului este o eroare sau nu
                si afisez mesajele corespunzatoare pentru fiecare caz.

        get_books - Functioneaza asemanator cu connection, intrucat am inclus si logica
                pentru afisarea informatiilor pentru o singura carte, cat si pentru
                toate cartile din librarie.
                -Daca se dorea accesarea pentru o singura carte, am adaugat la url si
                id-ul trimis ca parametru.

        delete_book - Tot ca si in functiile anterioare, creez cererea pentru server(adaug
                    id-ul ca si pentru comanda get_book) si interpretez raspunsul.

find.c - find_error - Am parcurs raspunsul de la server pana la primul '\n' (de la capatul
                    raspunsului) si i-am memorat pozitia. Apoi, intr-un vector de char-uri
                    am memorat eroarea si am marcat si terminatorul de sir.
