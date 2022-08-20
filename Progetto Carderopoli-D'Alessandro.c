#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIM_POST 1001 //Dimensione stringa dei post
#define DIM_NOME 21 //Dimensione stringa delle password/nomi utente

//Struct lista dei post (stringhe)
typedef struct post
{
    char post[DIM_POST];
    struct post *older;
}Post;

//Struct lista dei seguiti (stringhe)
typedef struct seguiti
{
    unsigned long long int uid;
    struct seguiti *next;
}Seguiti;

//Struct lista di tutti gli utenti registrati
typedef struct lista_utenti
{
    unsigned long long int uid; //Id numerico assegnato (non modificabile)
    char id[DIM_NOME]; //Nome utente scelto dall'utente (modificabile)
    char pw[DIM_NOME]; //Password scelta dall'utente (forse modificabile)
    Post *profilo_utente; //Lista contenente i post dell'utente (max 10)
    Seguiti *amicizie; //Lista contenente gli uid degli utenti seguiti
    struct lista_utenti *next; //Pointer all'utente successivo
    int numpost; //Numero di post scritti dall'utente

}Lista_utenti;

Lista_utenti *current; //Pointer alla lista degli utenti per scorrerli per le varie funzionalità
Lista_utenti *session; //...                            ...  utilizzare l'account scelto

//Contatore globale per l'uid dei vari utenti
unsigned long long int next_useruid = 6;

//Funzione per il controllo degli input nei menu
void Input_check(char *controllo);

//Funzione per i 5 utenti standard
void Utenza();

//Funzione creazione di un nuovo account
void Crea_account();

//Funzione per il login
void Login();

//Funzione per il logout
void Logout();

//Funzione per index
void Index();

//Funzione per login effettuato
void Session();

//Funzione per controllare se il nome utente è presente nella lista di utenti (Ritorna un puntatore all'utente cercato o NULL in sua assenza)
Lista_utenti *Controllo_Nome(char *checkname);

//Funzione controllo spazi
char SpaziOrTabs(char *stringa); //Ritorna 1 se ci sono spazi, 0 altrimenti

//Funzione che fa visualizzare l'ultimo post di ogni utente seguito
void Up_me_to_date ();

//Funzione per cercare un utente nel social network
void Ricerca();

//Stampa la lista dei seguiti dell'utente attuale
void Scorri_lista_seguiti();

//Funzione che ti porta alla tua pagina dove puoi postare, modificare il tuo username o cancellare l'account
void My_page ();

//Funzione che permette di seguire un utente
void Follow(unsigned long long int uid);

//Funzione per smettere di seguire un utente
void Unfollow(unsigned long long int uid);

//Funzione per controllare l'effettiva presenza tra i seguiti di un utente
int Controllo_follow(unsigned long long int uid);

//Funzione per scrivere i post
void New_post(char *frase);

//Funzione che si assicura della corretta scrittura dei post
void Post_check(char *testo);

//Funzione che serve a prendere l'indirizzo di un utente in memoria tramite l'uid
Lista_utenti* Utente_in_lista(unsigned long long int uid);

//Funzione che modifica il nome dell'utente
void Modifica_nome();

//Funzione per eliminare un account
void Elimina_account();

//Funzione che mostra tutti gli utenti iscritti al social network
void Visualizza_utenti();

//Funzione che controlla se il nome è troppo lungo
void Nome_check(char *nome);

//Funzione che controlla che la password rispetti le dimensioni
void Pw_check(char *password);

//Funziona che elimina i post di un account eliminato
void Elimina_seguiti();

//Funzione che elimina la lista seguiti di un account eliminato
void Elimina_post();

//Funzione per pulire lo schermo a ogni menu diverso
void clear();

//Funzione per stampare i post
void Stampa_post(char *testo);

//Funzione per modificare la password
void Modifica_pw();

//Inizio del programma
int main()
{
    Utenza();
    Index();

    return 0;
}//Fine del programma

void Index()
{

    char scelta;
    printf("\t\tBenvenuto su UnknownFriends, cosa desideri fare?\n");

    do
    {
        printf("1. Crea un nuovo account\n2. Effettua il login in un account esistente\n0. Chiudi UnknownFriends\n");
        Input_check(&scelta);

        switch (scelta)
        {
            case '1': //Crea account

                Crea_account();
            break;

            case '2': //Login

                Login();
            break;

            case '0'://Uscita dal ciclo

                printf("Ricorda di non stare troppo tempo incollato allo schermo! Prenditi delle pause.\n");
            break;

            default: //Valore non accettabile

                printf("ERRORE: comando non riconosciuto\n");
            break;

        }

    } while (scelta!='0'); //Ciclo principale di scelta dell'index

}

char SpaziOrTabs(char *stringa)
{
    int i = 0; //Variabile che scorre la stringa
    char CiSono = '0'; //variabile che funge da flag


    while(*(stringa+i)!='\0' && CiSono == '0') //Se si trovano spazi o tab oppure la stringa termina
    {
        if(*(stringa+i)=='\t' || *(stringa+i) == ' ') //Si controlla il carattere attuale e se è uno spazio o un tab
        {
            CiSono = '1'; //Cisono diventa 1 e si può uscire dal ciclo
        }
        i++; //Si passa al carattere successivo
    }

    return CiSono; //Si passa il valore di CiSono (0 non ci sono spazi, 1 ce ne sono)

}

void Nome_check(char *nome)
{
    char controllo[1000]; //Stringa di controllo e buffer
    int pass; //Variabile di passaggio

    do
    {
        printf("Nome utente: ");
        gets(controllo); //Si prende la stringa da tastiera
        if(strlen(controllo)>20) //Se la lunghezza è maggiore di 20
        {
            printf("ERRORE: Questo nome utente e' troppo lungo. Riprovare.\n");
            pass = 0; //Si reitera i do while
        }
        else //Altrimenti
        {
            pass = 1; //Si può procedere
        }
    }while (pass == 0);

    strcpy(nome,controllo); //Si copia controllo in nome

}

void Pw_check(char *password)
{
    char controllo[1000]; //stringa di controllo
    int pass = 1; //variabile di passaggio

    do
    {
        printf("Password: ");
        gets(controllo); //Si prende la password da tastiera
        if(strlen(controllo)<8 || strlen(controllo)>20) //Si controlla la sua lunghezza
        {
            if(strlen(controllo)!=1 || controllo[0]!= '0' ) //Negazione di (Se è lunga 1 e consta del carattere 0)
            {
                printf("ERRORE: Password troppo corta o troppo lunga. Riprovare. Inserire 0 per annullare l'inserimento\n");
                pass = 0; //Si reitera il tutto
            }
            else pass = 1;   //Si esce dal ciclo
        }
        else pass = 1; //si esce dal ciclo

    } while (pass == 0);


    strcpy(password,controllo); //Si copia controllo in password

}

void Crea_account()
{
    char nome_utente[DIM_NOME]; //Nome utente alla registrazione
    char password[DIM_NOME]; //Password alla registrazione
    char uscita; //Variabile necessaria all
    Lista_utenti *controllo;
    char err = '0'; //Variabile per controllare la presenza di errori nel processo di registrazione (0=falso, 1=vero)

    clear();
        do
        {
            do
            {
                printf("1. Inserisci un nome utente\n0. Torna al menu principale\n\n");
                Input_check(&uscita); //Controlla se ciò che viene inserito è effettivamente una delle opzioni
                switch (uscita)
                {
                    case '1':
                        clear();
                        printf("Inserisci un nome utente, MAX 20 caratteri (non sono accettati spazi)\n");
                        Nome_check(nome_utente); //Prende il nome desiderato in input
                        err = SpaziOrTabs(nome_utente); //Controllo della presenza di tabs o spazi
                        if(strlen(nome_utente)==0 ) err = '1'; //Inoltre se il nome utente è nullo, non verrà inserito
                    break;

                    case '0':
                        clear();
                        err = '0'; //In modo che il ciclo non si reiteri più
                        uscita = '2'; //Non va in nessun altro ciclo
                        printf("Sei nel menu' principale\n\n"); //Si chiuderà la function tornando nella index
                    break;

                    default:
                        clear();
                        printf("ERRORE: comando non riconosciuto\n");
                        err = '1';
                    break;
                }

                if(err == '1'&&uscita=='1')  //Messaggio di errore che compare solo nel caso in cui sono stati inseriti i caratteri non accettati, si è superata la lunghezza, o inserita la stringa nulla
                    {
                        clear();
                        printf("ERRORE: assicurarsi di:\n1 Aver rispettato la dimensione massima\n2 Non aver inserito caratteri non accettati (spazi o tab)\n\nRiprovare: \n\n");

                    }

            }while(err == '1'); //Il ciclo si itera finché l'utente inserisce un nome correttamente oppure decide di uscire

            if(uscita == '1') //Uscita sarà 0 solo nel caso in cui nella selezione precedente l'utente ha inserito 0
            {
                controllo = Controllo_Nome(nome_utente); //Controllo se esiste un utente con quel nome specifico

                if(controllo == NULL) //Se è null, non c'è alcun utente con quel nome, per cui si può procedere
                {
                    err = '0';//err è 0, per cui si esce dall'inserimento nome e si passa alla password
                }
                else
                {
                    clear();
                    printf("Il nome utente e' gia' stato preso, riprovare \n \n");
                    err = '1'; //Se non è null, esiste un utente con quel nome, e bisogna inserirne uno diverso
                }
            }

        }while (err == '1');

    err = '0'; //Il messaggio di errore non deve comparire al primo ciclo
    while (uscita == '1') //Uscita non viene più cambiata se l'utente inserisce un nome utente, e rimanendo 1 significa che ha senso portarlo alla schermata di inserimento password
    {
        do
        {
            printf("1. Inserisci una password\n0. Torna al menu principale (non verra' registrato alcun account)\n\n");
            Input_check(&uscita);
            switch (uscita)
            {
            case '1':

                uscita = '0'; //In questo modo si accede al blocco di istruzioni per il push del nuovo nodo utente in current (che è la lista di tutti gli utenti registrati)
                Pw_check(password);//Si prende una password
                if (strlen(password)==1 && password[0]=='0') uscita = '2';
                err = SpaziOrTabs(password);//Solito controllo di spazi oppure caratteri \t
                if (strlen(password)<8 || strlen(password)>20) err = '1'; //e se non si rispettano le dimensioni, avremo un messaggio di errore
            break;

            case '0': //Uscita=2 fa uscire dal ciclo senza entrare nella registrazione dell'account
                uscita='2';
                clear();
                printf("Sei nel menu' principale\n\n");
                err = '0'; //Ci farà uscire dal ciclo
            break;

            default:
                clear();
                printf("ERRORE: comando non riconosciuto\n\n");
                err = '1';
            }

            if(err == '1' && uscita == '0')
            printf("ERRORE: Password invalida, controllare di non aver inserito caratteri non accettati e di rispettare le dimensioni(min 8, max 20)\n\n");

            if(strlen(password)== 1 && password[0] == '0')
            {
                clear();
                printf("Sei nel menu' principale\n");
                err = '0';
                uscita = '2';
            }
        } while (err == '1'); //Fine ciclo inserimento password
    }

    if(uscita == '0') //Uscita è 1 solo se l'utente inserisce una password corretta
    {        //Inizio istruzioni per la creazione dell'utente
        controllo = (Lista_utenti *) malloc(sizeof(Lista_utenti)); //si alloca memoria a controllo per un nuovo nodo dedl tipo lista_utenti
        controllo->uid=next_useruid++; //Prende l'uid, che dopo viene incrementato
        strcpy(controllo->id, nome_utente);//Assegnazione del nome
        strcpy(controllo->pw, password);//Assegnazione della password
        controllo->profilo_utente = NULL; //Inizializzazione della lista profilo_utente (i suoi post)
        controllo->next = current; //Puntatore all'elemento successivo (il vecchio utente più recente, contenuto in current)
        controllo->amicizie = NULL; //Inizializzazione della lista seguiti
        controllo->numpost = 0; //Inizializzazione counter dei post dell'utente (ovviamente sono 0)
        current = controllo; //Current punta all'account più recente (quello appena registrato)
        clear();
        printf("Registrazione effettuata con successo! Effettua il login.\n\n");
    }  //Fine istruzioni per la creazione dell'utente

}

void Login() //Funzione per il login di un utente esistente
{
    char nome_utente[DIM_NOME]; //Nome utente (Da ricercare nella lista)
    char password[DIM_NOME]; //Password (Controllare se corrisponde al nome utente)
    char next_step; //Variabile per la gestione dei cicli
    char ciclo_password; //Gestione del ciclo della password
    char uscita; //Se l'utente vuole uscire diventa 1
    Lista_utenti *account_controllato;

    clear();
    do{ //Inizio ciclo inserimento nome

        next_step='0';
        uscita='0';

        printf("1. Inserisci il tuo nome utente\n");
        printf("0. Esci dalla schermata di login\n");
        Input_check(&uscita);

        switch(uscita){

            case '1': //L'utente vuole inserire un nome

                uscita='0';
                Nome_check(nome_utente);

                account_controllato=Controllo_Nome(nome_utente);

                if (account_controllato!=NULL){ //Esiste un account con quel nome

                    next_step='1'; //Interrompe il ciclo esterno

                    do{ //Inizio ciclo password

                        ciclo_password='0';

                        printf("Inserisci la password\n");
                        Pw_check(password);

                        if(strcmp(password, (account_controllato->pw))==0)
                        { //Se password inserita e password dell'account trovato coincidono

                            printf("Login effettuato correttamente\n");
                            session=account_controllato; //Rende l'account da controllare nella sessione quello a cui si è accesso
                            Session();

                        } //Fine caso password coincidenti

                        else if ((strlen(password)==1)&&(password[0]=='0')) //Se la password è lunga un carattere e consta del carattere 0
                        {
                            clear();
                            printf("Accesso annullato correttamente\n");
                            uscita = '1';
                        }

                        else { //Se non coincidono

                        printf("La password non coincide con l'account, cosa vuoi fare?\n");
                        printf("1. Riprova con un'altra password\n");
                        printf("2. Accedi ad un altro account\n");
                        printf("3. Esci dalla schermata di login\n");
                        Input_check(&ciclo_password);

                        if (ciclo_password<'1'||ciclo_password>'3'){ //Input menu ciclo password errato

                            printf("ERRORE: il valore inserito non e' accettabile\n");
                            ciclo_password='1';

                        }

                        } //Fine caso password non coincidenti

                    } while (ciclo_password=='1'); //Fine ciclo password

                    if (ciclo_password=='3') uscita='1'; //Garantisce l'uscita dal ciclo

                } //Fine caso account esistente

                else{ //Se il nome inserito non esiste

                    printf("ERRORE: Il nome utente inserito non e' esistente\n");

                }//Fine caso nome non esistente

            break;

            case '0':

                clear();

                printf("Sei tornato al menu\n");

                uscita='1'; //Garantisce l'uscita dal ciclo
            break;

            default:

                uscita='0';
                printf("ERRORE: comando non riconosciuto\n");
            break;

        } //Fine switch

    } while (((next_step=='0')||(ciclo_password=='2'))&&(uscita=='0'));
    /*Fine ciclo inserimento nome (next_step diventa 1 se l'utente trova un nome esistente
    ciclo_password diventa 1 quando l'utente vuole reinserire la password e 0 se riesce ad accedere
    uscita diventa 1 se l'utente vuole uscire*/

}

void Post_check(char *testo)
{
    char controllo[2001];
    int err=0;
    int i,c;

    do
    {
        c=0;
        i=0;
        gets(controllo); //Si prende la stringa in input dall'utente
        while(i==0 && controllo[c]!='\0')
        {
            if(controllo[c]!=' ' && controllo[c]!='\t') i++; //Se esisterà almeno un carattere diverso da  \t e ' ' allora i diventerà 1.
        }

        if(strlen(controllo)>1000 || strlen(controllo)==0 || i==0) //In caso sia la stringa nulla, la dimensione sia maggiore di 1000 oppure non ci sono caratteri diversi da tabs e spazi, si da un errore
        {
            printf("ERRORE: Il post non e' valido, riprovare\nAccertarsi di inserire almeno un carattere diverso da spazi\nE di aver inserito meno di 1000 caratteri.\n\n");
        }

    } while (err=='0');

    strcpy(testo,controllo); //si copia la stringa in controllo direttamente nell'indirizzo a cui punta testo

}

Lista_utenti *Controllo_Nome(char *checkname)
{

    Lista_utenti *utenteattuale=current; //Puntatore per scorrere la lista di utenti

    while (utenteattuale!=NULL){

        if (strcmp((utenteattuale->id), checkname)==0) return utenteattuale; //Se trova il nome ritorna il puntatore all'utente

        utenteattuale=utenteattuale->next; //Reitera

    }

    return NULL; //Se non lo trova ritorna NULL

}

void Logout ()
{

    printf("Logout effettuato correttamente\n");
    session=NULL; //In modo tale da terminare anche la funzione Session() e tornare a Index()

}

void Input_check(char *controllo)
{
    int err=0; //Indica se ci sono errori nell'inserimento dell'input
    char Controllo[1000]; //Stringa di controllo

    do
    {
        if(err==1)
        {
            printf("ERRORE: comando non riconosciuto\n");
            printf("Riprovare con uno valido\n");
        }
        gets(Controllo); //Input dell'utente fino al carattere \n
        if(strlen(Controllo)!=1) //Se la stringa ha lunghezza diversa da 1 significa che l'utente non ha inserito un carattere bensì una parola o la stringa nulla
        {
            err = 1; //Si reitererà la funzione
        }
        else
        {
            err = 0;//Si uscirà dal ciclo
            *controllo = Controllo[0]; //L'indirizzo in memoria dato come input della function prende il valore del primo elemento di Controllo
            //(se infatti scrivo 2\n la dimensione sarà 1 e il carattere sarà 2)
        }

    }while(err==1);

}

void Session()
{
    char scelta;

    clear();

    printf("Benvenuto ");
    puts(session->id);

    while(session!=NULL) //Fino a quando l'utente è in session
    {

        printf("\nCosa desideri fare?\n1.Aggiornare il feed (Vedere il post piu' recente degli utenti che segui)");
        printf("\n2.Cercare un utente per nome\n");
        printf("3.Controllare la lista degli utenti iscritti\n");
        printf("4.Controllare la lista degli utenti che segui\n");
        printf("5.Accedere alla tua pagina\n");
        printf("0.Log out (Verrai ricondotto alla pagina iniziale)\n");

        Input_check(&scelta); //Prende l'input della scelta e verifica se è accettabile

        switch (scelta)
        {
            case '1':

                clear();
                Up_me_to_date();
            break;

            case '2':

                clear();
                Ricerca();
            break;

            case '3':

                clear();
                Visualizza_utenti();
            break;

            case '4':

                clear();
                Scorri_lista_seguiti();
            break;

            case '5':

                clear();
                My_page();
            break;

            case '0':

                clear();
                Logout();
            break;

            default:

                clear();
                printf("ERRORE: comando non riconosciuto\n");
            break;
        }
    } //Fine while

}

void New_post(char *frase)
{
    Post *deletePrev;
    Post *delete; //Sono i due pointer che andranno ad esplorare la lista dei post
    Post *new=(Post*)malloc(sizeof(Post)); //Allocazione della memoria per il push

    if(session->numpost<10) //Se non si è ancora raggiunto il limite di post massimi
    {
        strcpy(new->post,frase); //allora si procede ad un semplice push.
        new->older=session->profilo_utente; //New->older punterà alla testa attuale
        session->profilo_utente=new; //e a sua volta la testa attuale punterà al nuovo indirizzo di memoria.
        session->numpost++; //Si incrementa il numero di post
    }
    else //Se si è raggiunto il limite
    {
        delete = session->profilo_utente;
        deletePrev = delete; //Avremo delete che punterà inizialmente allo stesso valore di deletePrev, ma alla fine
        while(delete->older!=NULL)//di questo ciclo while, deletePrev punterà al post precedente all'ultimo post
        {
            deletePrev = delete; //perché l'ultima assegnazione che si avrà sarà deleteprev punta al penultimo nodo
            delete = delete->older;//delete punta all'ultimo nodo (older punta a NULL)
        }
        deletePrev->older = delete->older; //uscito dal ciclo while, dobbiamo "tagliare" il nodo contenuto in delete, dicendo a deletePrev->older di puntare al nodo dopo di delete (che in realtà è NULL, dunque non è un nodo)
        free(delete); //Si dealloca la memoria in delete (Il vecchio ultimo post)
        strcpy(new->post,frase); //Dopo di che si procede all'usuale push, quindi si copia la stringa
        new->older=session->profilo_utente;//si va ad assegnare al pointer dell'indirizzo in "new" il nodo in testa attuale
        session->profilo_utente=new;//ed infine il pointer profilo utente in session sarà uguale a new.
    }
}

void Scorri_lista_seguiti() //Stampa la lista degli utenti seguiti dall'user in session
{
    Seguiti* scorri=session->amicizie; //Ci serve questo pointer per esplorare la lista dal primo all'ultimo nodo
    Seguiti* del;
    Lista_utenti* utenteattuale; //Andrà a prendere di volta in volta gli indirizzi dei vari utenti seguiti tramite il suo uid
    int follcounter=0; //Counter dei seguiti per gestire i cicli
    int clean; //Indica se la lista dei seguiti è stata ripulita dagli account cancellati


    if(scorri==NULL) //Se non ci sono
    {
        printf("Non segui ancora nessuno oppure gli utenti che seguivi hanno eliminato il loro account.\n");
        follcounter=1; //Per non ottenere il secondo messaggio di errore
    }
    else
    {
        while(scorri!=NULL) //Finchè non finiscono gli utenti seguiti
        {
            if(Utente_in_lista(scorri->uid)!=NULL) //Si controlla se l'utente in lista cercato tramite uid esiste effettivamente, altrimenti l'account è stato eliminato
            {
                if(follcounter==0) //Se si è trovato almeno un account seguito non eliminato (Così il messaggio non è ripetuto per ogni ciclo)
                {
                    printf("Ecco gli utenti che ti interessano:\n");
                }
                utenteattuale=Utente_in_lista(scorri->uid); //Utenteattuale prende l'indirizzo dell'utente con l'uid corrispondente
                puts(utenteattuale->id); //Si scrive il nome dell'utente
                putchar('\n');
                scorri = scorri->next; //Si manda scorri al prossimo nodo
                follcounter++; //si aumenta il counter dei seguiti, altrimenti otterremo un messaggio di errore
            }
            else //Se l'account è stato eliminato
            {
                scorri->uid=0; //L'uid è azzerato
                scorri = scorri->next; //La lista prosegue
            }
        }

        do
        {
            clean = 1; //La lista è pulita
            del = session->amicizie; //Del e scorri puntano inizialmente allo stesso nodo, che sarebbe il primo della lista seguiti
            scorri=del;
            while(del!=NULL && del->uid!=0) //Il ciclo porta del come nodo di lista seguente a scorri ed esce quando del incontra l'uid nullo o termina la lista
            {
                scorri = del;
                del = del->next; //In questo modo del si troverà "avanti" a scorri o meglio del==scorri->next
            }
            if(del!=NULL) //Se l'uid corrente di del è 0
            {
                if(del == scorri) //Se del == scorri significa che del==session->amicizie, ossia bisogna eseguire un pop agendo su session->amicizie stesso
                {
                    session->amicizie=del->next; //Session->amicizie punterà all'elemento successivo
                    free(del); //Deallochiamo la memoria del nodo da rimuovere
                    clean = 0; //Facciamo reiterare le istruzioni
                }
                else //Altrimenti significa che scorri punta al nodo "precedente" a del, per cui bisogna tagliare del fuori dalla lista
                {
                    scorri->next=del->next; //Si "taglia" del dalla lista, facendo puntare all'elemento in scorri l'elemento dopo next
                    free(del); //Si dealloca la memoria
                    clean = 0; //E si dice al programma che bisogna reiterare le istruzioni, finché tutti gli utenti che hanno eliminato il proprio account saranno rimossi dalla lista seguiti
                }
            }
        } while (clean==0);

    }

    if(follcounter==0) //Se la lista non è vuota, ma comunque non sono stati trovati utenti
    {
        printf("Non segui ancora nessuno oppure gli utenti che seguivi hanno cancellato il loro account.\n");
    }

}

void Utenza()
{
    Lista_utenti *new;//pointer del nuovo account
    char post[DIM_POST];
    Seguiti *follower;

    //Primo utente inizializzato
    new = (Lista_utenti*) malloc(sizeof(Lista_utenti)); //Allocazione del nuovo nodo
    new->uid = 1; //I primi 5 account avranno id numerico da 1 a 5
    strcpy(new->id, "Ferdinando");
    strcpy(new->pw, "FFFFFFFF");
    new->amicizie=NULL;
    new->profilo_utente=NULL;
    new->next= NULL;
    current = new;
    current->numpost=0;
    session=current; //Rendo l'utente quello in sessione così da poter utilizzare le funzioni di creazione post e follow
    New_post("buongiornissimo caffe'... oggi pulizia contatti!!! :D");
    New_post("Anche oggi vedo solo gente senza mascherina... che posto incivile! >:(");
    New_post("Domani, mi faccio la millefoglie al caffe'! :)");
    New_post("Oramai, non ho + parole!... -_-");
    New_post("auguroni di un buon 2021! ^-^");
    New_post("Grazie agli amici che si sono preoccupati per me... Il peggio e' passato!\nPer chi non lo sapesse, ieri pomeriggio avevo deciso di provare a fare un giro a cavallo da dei miei amici.\nImprovvisamente il cavallo ha iniziato ad andare sempre piu' veloce.\nAll'improvviso sono caduto ma il piede si e' incastrato nella staffa ed il cavallo mi ha trascinato con se'.\nDevo ringraziare il proprietario e il funzionario hanno subito spento la giostra,\naltrimenti sarei stato investito dal camion dei pompieri, dalla giraffa, dalla moto, dall'aereo e dal polipo.\nComunque ora sto bene!!\nSe hai letto fino a qui forse e' perche' ti sei preoccupato per me. \nNon condividere, fai copia e incolla e fai ridere altri. Perche' ridere e' la migliore medicina!!! ...e mandami pure a quel paese!!!! xD xD xD xD xD");
    New_post("C'e' talmente tanto traffico che quello affianco a me ha chiesto 'CheVuoPeNatal' xD xD");
    New_post("Grazie amore mio della bellissima sorpresa, sei una grande donna TI AMO <3 <3 <3 <3 <3");
    New_post("amo svegliarmi ogni giorno vicino all'amore della mia vita... il letto!! xD condividi se ti ha fatto ridere!!");
    New_post("Vendo SH300 colore grigio motore nuovissimo,senza un graffio con casco in regalo....PREZZO POCO MA POCO TRATTABILE!!!! No affaristi e perditempo");
    current->numpost=10;

    //Secondo utente inizializzato
    new = (Lista_utenti*)malloc(sizeof(Lista_utenti));
    new->uid = 2;
    strcpy(new->id,"Pasquale");
    strcpy(new->pw,"PPPPPPPP");
    new->amicizie=NULL;
    new->profilo_utente=NULL;
    new->next=current;
    current=new;
    current->numpost=0;
    session=current; //Rendo l'utente quello in sessione così da poter utilizzare le funzioni di creazione post e follow
    Follow(1);
    New_post("Festeggiamo insieme i 5 iscritti al nostro social! Un traguardo unico nel suo genere!");
    New_post("Hey RandomFax_BOT, metti Despacito. Ah no, purtroppo non e' come Axel...");
    New_post("@Ferdinando che ne pensi di aggiungere anche le UnknownStories?");
    New_post("Come allontanarsi dalla dipendenza da social network:\n1. Vai nella tua pagina\n2. Vai in \"Modifica password\"\n3. Inserire qualcosa del tipo hnuyj845tq08yn08yb41q50 e dimenticarla per sempre\n4. Fine\n");
    New_post("(Ne io ne @Ferdinando ci assumiamo alcuna responsabilita' della perdita delle vostre credenziali)");
    New_post("@xXDarkBeastXx sto rimpiangendo di averti creato, mi provochi dispiacere.");
    New_post("Probabilmente dovrei smetterla di perdere tempo sui social e completare il progetto di programmazione...");
    New_post("@BulletKey bro non capisco perche' dovrebbe importarmi il modo in cui sono salvato nella tua rubrica, ti prego illuminami");
    New_post("Se potessi scegliere chi essere, sarebbe me stesso. O Jezos Beff, tanto stiamo sullo stesso livello.");
    New_post("Lo sapevi? Unknown friends significa \"amici sconosciuti\". Adesso faccio competenza anche alle pagine di notizie.");
    current->numpost=10;

    //Terzo utente inizializzato
    new = (Lista_utenti*)malloc(sizeof(Lista_utenti));
    new->uid = 3;
    strcpy(new->id,"BulletKey");
    strcpy(new->pw, "BBBBBBBB");
    new->amicizie=NULL;
    new->profilo_utente=NULL;
    new->next=current;
    current=new;
    current->numpost=0;
    session=current;//Rendo l'utente quello in sessione così da poter utilizzare le funzioni di creazione post e follow
    Follow(1);
    Follow(2);
    New_post("Oggi settimana 8 di FNF, comunque piu' interessante dei teoremi");
    New_post("@Pasquale ormai sei segnato Ke.in meno di un sottospazio vettoriale ahahha");
    New_post("E' divertente scrivere nella rubrica solo che poi va a mio discapito");
    New_post("Domanda, quando devo pagare le tasse? Tanto non voglio farlo con questo isee altissimo");
    New_post("@Ferdinando a quanti quaderni stai? Io mezzo ahah");
    New_post("Prendere appunti e' proprio sopravvalutato ahhah");
    New_post("Ma che povero, stev na favol hahaahahha");
    New_post("Voi che siete intelligenti, se la capute siete ottimi assistenti");
    New_post("Percio' chiedevo, sto esame non ha un senso logico in online");
    New_post("Anche oggi vedo solo teoremi inutili, quando finira' questo strazio?\nsono venuto qui per programmare non per studiare ._.");
    current->numpost=10;

    //Quarto utente inizializzato
    new = (Lista_utenti*)malloc(sizeof(Lista_utenti));
    new->uid = 4;
    strcpy(new->id,"xXDarkBeastXx");
    strcpy(new->pw,"XXXXXXXX");
    new->amicizie=NULL;
    new->profilo_utente=NULL;
    new->next=current;
    current=new;
    current->numpost=0;
    session=current; //Rendo l'utente quello in sessione così da poter utilizzare le funzioni di creazione post e follow
    Follow(1);
    Follow(2);
    New_post("Dopo essere stato bannato da reddit e twitter, eccomi qui. Un nuovo inizio :D\n");
    New_post("A A A\n Cerco compagno di giochi. Deve rispettare le seguenti proprieta':\n1.Essere una ragazza.\n");
    New_post("Ragazzi, qualcuno qui se ne intende del mondo degli e-sports?? Se si potrebbe contattarmi?\nMi servono vari consigli su come convincere i miei che sia un buon investimento di denaro e tempo.\n");
    New_post("Devo assemblare il mio nuovo pc, ma ovviamente cosa poteva mai mancare?\nMaledetti miner, che si tengano pure le loro GPU. >.<'");
    New_post("Tra le 7 miliardi e mezzo di persone, proprio tu hai avuto l'onore di leggere questo post.\nTi auguro una buonissima giornata ^-^ \n(no, non ho proprio nulla da fare)");
    New_post("Si ragazzi, vi voglio bene etc., pero' non potete dirmi che Tifa non sia meglio di Farron.\nDai, bisogna essere onesti");
    New_post("Domani primo appuntamento. Non mi sentivo cosi' da molto tempo, volevo condividere con voi la mia gioia.\nCerto non e' da me parlare di certi argomenti, ma c'e' sempre una prima volta no?\n");
    New_post("E' veramente triste nel 2021 vedere persone che continuano a sostenere che i videogiochi facciano male.\nNo ragazzi, siete voi che fate male, ridicoli.\nNiente di personale ovviamente, pace.");
    New_post("@BulletKey ma quanto e' figo fnf?\nSto giocando la week 8 e sto letteralmente rischiando di essere bocciato all'esame di domani.");
    New_post("Ho appena ricevuto la mia seconda dose di vaccino. Il dottore ha notato il mio tatuaggio di Jinx guardiana stellare, e subito dopo mi ha dato il suo nickname. Che boomer.");
    current->numpost=10;

    //Quinto utente inizializzato
    new = (Lista_utenti*)malloc(sizeof(Lista_utenti));
    new->uid = 5;
    strcpy(new->id,"RandomFax_BOT");
    strcpy(new->pw,"01010101");
    new->amicizie=NULL;
    new->profilo_utente=NULL;
    new->next=current;
    current = new;
    current->numpost=0;
    session = current; //Rendo l'utente quello in sessione così da poter utilizzare le funzioni di creazione post
    New_post("L'anticiclone africano sta scaldando i motori e si appresta a invadere buona parte d'Italia a partire dal weekend.\nIl team del sito www.sitoFittizioSulMeteo.it informa che sabato e domenica saranno due giornate in gran parte asciutte e soleggiate,\nci saranno soltanto alcuni temporali pomeridiani sull'arco alpino.\nSeguimi per altre notizie interessanti :D");
    New_post("Negazionista del Covid non credeva alla pandemia, ma una volta contagiato e' stato curato per tre mesi\ne dopo la guarigione ha scritto di suo pugno una lettera ai medici per ringraziarli.\nMa non e' finita qui, ha deciso di fare loro un regalo per il reparto.\n\"Devo la mia vita a tutti gli operatori sanitari che non mi hanno mai lasciato solo,\nmi sono stati vicino con le medicine e con la loro grande umanita', come se fossi uno di famiglia.\nLa sanita' pubblica e' il nostro bene piu' prezioso: dobbiamo difenderla con unghie e denti!\".\nSeguimi per altre notizie interessanti :D");
    New_post("Dopo quasi 50 giorni di calma, nella notte tra martedi' 18 e mercoledi' 19 maggio, sull' Etna, un nuovo parossismo ha interessato il solito cratere di Sud Est.\nLa fase eruttiva non ha avuto alcun impatto sull'operativita' dell'aeroporto internazionale di Catania.\nSeguimi per altre notizie interessanti :D");
    New_post("Decine di milioni di api letteralmente scomparse nel nulla in meno di un anno e solo in Lombardia.\nE la stima e' al ribasso perche' prende in considerazione solo le api monitorate dagli apicoltori, quindi mancano all'appello gli impollinatori selvatici.\nSeguimi per altre notizie interessanti :D");
    New_post("In commercio si trovano diverse tipologie di spray depilatorio,\nma il migliore, secondo i consumatori e gli esperti del settore, e' Hair Erase che permette di sbarazzarsi dei peli senza dolore.\nUno spray che permette di liberarsi dalla peluria in eccesso in sei settimane circa. Molto facile e sicuro da usare.\nSi compone di elementi naturali che non hanno controindicazioni o effetti collaterali.\nSeguimi per altre notizie interessanti :D");
    New_post("Lo sapevi? Questo social network e' stato creato da Pasquale e Ferdinando! Prova a cercarli!\nSeguimi per altre notizie interessanti :D");
    New_post("Dall'alba del sabato mattina le parenti di San Gennaro, si chiamano cosi' le devotissime, non si sono perse d'animo,\npregano, invocano, recitano il rosario di San Gennaro. Ma c'e' chi sbuffa e bofonchia:\n\"San genna' o ci pensi tu e ci togli 'sto covid da mezzo, o salta pure 'sta volta la processione...\".\nIntanto il busto d'oro del Santo, da qui il nomignolo faccia gialla, era stato trasferito dalla cappella del Tesoro all'altare.\nIl grumo di sangue raccolto nella teca era un sasso, poi verso le 5 del pomeriggio di domenica ha incominciato a sciogliersi.\nSeguimi per altre notizie interessanti :D");
    New_post("Una gigantesca lastra di ghiaccio piu' grande della isola spagnola di Maiorca si e' staccata dall'Antartide nel Mare di Weddell,\ndiventando il piu' grande iceberg del mondo attualmente a galla.\nSeguimi per altre notizie interessanti :D");
    New_post("Dal 1960 ad oggi la popolazione mondiale e' incrementata di 4 miliardi.\nSeguimi per altre notizie interessanti :D");
    New_post("Lo sapevi? Questo social network e' basato su 1488 righe di codice! ;)\nSeguimi per altre notizie interessanti :D");
    current->numpost=10;
    session = NULL; //Fa tornare l'utente attualmente in sessione a NULL
}

void Follow(unsigned long long int uid)
{
    Seguiti *nuovo = (Seguiti *) malloc(sizeof(Seguiti)); //Si alloca la memoria per il nuovo utente seguito

    nuovo->uid = uid; //L'uid sarà ovviamente quello dell'utente da seguire (che riceve in input)
    nuovo->next = session->amicizie; //L'elemento successivo diventerà il vecchio primo elemento
    session->amicizie = nuovo; //E l'head della lista si aggiorna

    if (session->uid>5) printf("La richiesta e' stata accettata dall'utente \n\n"); //Se l'utente non fa parte dei 5 inizializzati

}

void Unfollow(unsigned long long int uid)
{
    Seguiti *precedente=session->amicizie; //Serve per cambiare il campo next del seguito meno precedente
    Seguiti *successivo=precedente; //Serve per trovare l'uid dell'utente da cancellare

    while(successivo->uid!=uid)
    {
        precedente=successivo; //Precedente sarà graficamente sempre il nodo precedente di successivo
        successivo=successivo->next; //Per cui bisogna mandare successivo avanti, finche il suo uid sarà corrispondente
    }
    if(precedente == successivo)//Significa che bisogna eliminare il primo in lista, per cui è un vero e proprio pop
    {
        session->amicizie=session->amicizie->next; //Si assegna a session->amicizie il nodo successivo
        free(precedente);    //Si elimina il vecchio nodo
    }
    else
    {
        precedente->next=successivo->next; //Serve a "tagliare" il nodo contenuto in successivo
        free(successivo); //Si dealloca la memoria in successivo, cancellando quel nodo
        printf("Hai smesso di seguire questo utente\n\n");
    }


}

void Up_me_to_date()
{
    Seguiti *scorri=session->amicizie; //Scorri sarà un pointer con l'indirizzo del primo nodo della lista dei seguiti dell'utente in sessione
    Post *testo; //testo conterrà l'indirizzo della lista "profilo utente"o (e quindi l'ultimo post) di ogni utente
    unsigned long long int utente; //contiene l'uid che viene utilizzado da Utente_in_lista per ottenere il loro indirizzo in lista_utenti per poi poter accedere ai post e al suo id
    int postcounter=0;
    if (scorri==NULL) //Non ci sono persone seguite
    {
        printf("Non segui ancora nessuno oppure gli utenti che seguivi hanno cancellato il loro account.\n");
        postcounter = 1; //Altrimenti apparirà un secondo messaggio di errore
    }
    else
    {
        while(scorri!=NULL) //Scorrerà tutti i seguiti
        {
            utente = scorri->uid; //Si controlla l'effettiva esistenza dell'utente indicato nella lista seguiti
            if(Utente_in_lista(utente)!=NULL)//Se l'utente esiste allora Utente_in_lista riporterà il suo indirizzo ovviamente diverso da null
            {
                testo=Utente_in_lista(utente)->profilo_utente; //testo è un Post * dunque prenderà il valore del primo nodo dei post
                if(testo!=NULL) //Se esiste effettivamente la pagina dell'utente
                {
                    puts(Utente_in_lista(utente)->id); //Si stampa la stringa dell'utente che ha scritto il post
                    puts("\b ha scritto:\n");
                    Stampa_post(Utente_in_lista(utente)->profilo_utente->post); //Si stampa il post
                    puts("\n\n");
                    postcounter++; //il contatore dei post incrementa, altrimenti visualizzeremo un errore quando in realtà non ce n'è bisogno
                }
            }
            scorri = scorri->next; //si passa all'utente successivo
        }
    }

    if(postcounter==0)
    {
        printf("Non c'e' niente da vedere. Potrebbe essere per uno dei seguenti motivi:\n");
        printf("1. Non segui ancora nessuno oppure gli utenti che seguivi hanno cancellato il loro account\n\n2. Non ci sono post recenti\n");
    }

}

void Stampa_post(char *testo)
{
    int carattereattuale = 0; //Indice del carattere della stringa da stampare
    int max = 0; //numero massimo di caratteri oltre i 50 dove non compaiono spazi o new lines correnti
    int caratteriPzAtt= 0; //Lunghezza dell pezzo di post in scrittura
    int maxattuale = 0; //Numero massimo di caratteri oltre i 50 dove non compaiono spazi in generale (serve per formattare anche il resto del post)
    int numspazi = 0; //Indica quanti spazi prima di stampare ' *\n* ' sono necessari in modo da formattare il testo

    for( ; *(testo+carattereattuale)!='\0';carattereattuale++) //Inizio conta del numero di caratteri consecutivi senza spazi
    {
        max++;
        if(max >= 50)
        {
            if(*(testo+carattereattuale) != ' ' && *(testo+carattereattuale)!= '\0' && *(testo+carattereattuale)!='\t' && *(testo+carattereattuale)!='\n')
            { //Se incontra uno spazio o un carattere di tabulazione o terminazione
                {
                    if(maxattuale<max) maxattuale=max;
                }
            }
            else max = 0;
        }
       if(*(testo+carattereattuale)=='\n') //Se si incontra un newline
       {
           if(maxattuale<max)//Nel caso in cui maxattuale sia minore, bisogna aggiornarlo
           {
               maxattuale = max-1; //max contiene anche \n ma noi non vogliamo contarlo
           }
           max = 0; //Si contano i caratteri del nuovo pezzo di post
       }
    }//Fine conta
    if(maxattuale == 0) maxattuale = max; //Se non c'è alcun caso in cui ci sono piu di 50 caratteri, allora si prenderanno quelli piu grandi in generale

    for (int i = 0; i < maxattuale + 4; i++) printf("*"); //Stampa stelline contando 4 caratteri in piu
    printf("\n* ");
    for (carattereattuale = 0; *(testo+carattereattuale)!='\0'; carattereattuale++) //Inizio stampa post
    {
        caratteriPzAtt++;

        if(*(testo+carattereattuale)=='\n') //Se incontra un carattere newline
        {
            numspazi = maxattuale - (caratteriPzAtt-1); //Lo spazio finale di una riga non deve essere contato
            if(numspazi > 0) for(int j = 0;j < numspazi; j++) printf(" "); //Prende il numero di spazi da printare dopo la fine della riga per formattare correttamente
            printf(" *\n* "); //Printa la stellina di fine riga e quella di inizio riga seguente
            caratteriPzAtt = 0; //Si azzera il numero di caratteri del nuovo pezzo di post
        }

        else if(caratteriPzAtt>=50)
        {
            if(*(testo+carattereattuale)==' ' || *(testo+carattereattuale)=='\t' || *(testo+carattereattuale)=='\n')
            {
                numspazi=maxattuale-(caratteriPzAtt-1); //Lo spazio finale di una riga non deve essere contato
                if(numspazi > 0) for(int j = 0;j < numspazi; j++) printf(" "); //Prende il numero di spazi da printare dopo la fine della riga per formattare correttamente
                printf(" *\n* "); //Printa la stellina di fine riga e quello di inizio riga seguente
                caratteriPzAtt = 0; //Si azzera il numero di caratteri del nuovo pezzo di post
            }

            else //Altrimenti si stampa il carattere
            {
                putchar(*(testo+carattereattuale));
            }
        }

        else putchar(*(testo+carattereattuale));

    } //Fine stampa post


    numspazi = maxattuale - caratteriPzAtt; //formattazione ultima parte del post
    if(numspazi > 0)
    {
        for(int j = 0;j < numspazi; j++) printf(" "); //Stampa post necessari alla formattazione
    }
    printf(" *\n"); //Asterisco di fine rigo e newline
    for (int i = 0; i < maxattuale + 4; i++) printf("*"); //Stampa stelline di fine post
}

Lista_utenti *Utente_in_lista(unsigned long long int uid)
{
    Lista_utenti *scorri=current; //Si scorre la lista degli utenti dal più recente al più vecchio, dunque
 //abbiamo bisogno di questo pointer a current (struttura dove si trovano tutti gli user) per scorrerla

    while(scorri!=NULL && scorri->uid!=uid) //Si controllano gli utenti finché si trova l'uid corrispondente, o scorri sarà a null (avrà controllato ogni utente)
    {
        scorri=scorri->next; //scorri va avanti
    }

    return scorri; //Si restituisce il valore contenuto in scorri, che non è altro che un indirizzo all utente con quel uid (se non c'è, scorri sarà NULL)
}

void  Apri_pagina(Lista_utenti* user)
{
    Post *scorripost=user->profilo_utente; //scorripost avrà l'indirizzo della lista di post dell'utente corrente
    char scelta; //variabile per la scelta delle diverse funzioni

    printf("Stai guardando il profilo di ");
    puts(user->id); //stampa dell'username dell'utente corrente
    putchar('\n');

    if(scorripost==NULL) //Significa che non ci sono post nella pagina utente
    {
        printf("L'utente non ha ancora scritto alcun post. Riprova piu' tardi.\n");
    }
    else
    {
        printf("Ecco i post dell'utente (dal piu' al meno recente)\n\n\n");
        while(scorripost!=NULL)
        {
            Stampa_post(scorripost->post); //Di volta in volta scorripost andrà a stampare le stringhe (i post) dei nodi
            puts("\n\n");
            scorripost = scorripost->older; //Dopodiché passerà al nodo successivo (il post più vecchio)
        }
    }
    do
    {
        if(Controllo_follow(user->uid)== 1) //Controllo_follow restituisce 1 se l'utente in session segue l'utende con quel uid
        {
            printf("1. Non seguire piu' l'utente che stai visualizzando\n0. Torna alla home\n");
            Input_check(&scelta);
            switch (scelta)
            {
            case '1':

                Unfollow(user->uid); //smetterà di seguire (toglierà dalla lista seguiti) l'utente che si sta visualizzando
            break;

            case '0':

                printf("Sei nella home\n\n");
            break;
            default:

                printf("ERRORE: comando non riconosciuto\n");
            }
        }
        else
        {
            printf("1. Segui l'utente che stai visualizzando\n0. Torna alla home\n");
            Input_check(&scelta);
            switch(scelta)
            {
                case '1':

                    Follow(user->uid); //seguirà l'utente che si visualizza
                break;

                case '0':

                    printf("Sei nella home \n\n");
                break;

                default:

                    printf("ERRORE: comando non riconosciuto\n");
                break;
            }
        }
    }while(scelta !='0');

}

void Ricerca ()
{

    char nome_cercato[DIM_NOME]; //Contiene il nome che l'utente sta cercando
    char scelta; //Controlla se tornare nel menu o cercare un altro utente
    int uscita=0; //Controlla l'uscita dal ciclo esterno
    int errore;

    while(uscita==0){ //Ciclo esterno

        printf("Inserisci il nome dell'utente che stai cercando\n");
        Nome_check(nome_cercato);

        Lista_utenti *utente_cercato=Controllo_Nome(nome_cercato); //Assegna l'utente il cui nome è stato cercato

        if (utente_cercato==NULL){ //Se il nome non corrisponde ad alcun utente in rete

            printf("ERRORE: Il nome inserito non corrisponde ad alcun utente sulla rete\n");

            do{ //Inizio ciclo valore non accettabile

                printf("1. Cerca un altro utente\n");
                printf("0. Esci dalla ricerca\n");
                Input_check(&scelta);

                switch(scelta){

                    case '1':

                    errore=0;
                    break;

                    case '0':

                        errore=0;
                        uscita=1;
                    break;

                    default:

                        errore=1;
                        printf("ERRORE: comando non riconosciuto\n");
                    break;

                }//Fine switch

            }while (errore==1); //Fine ciclo valore non accettabile

        } //Fine caso utente non trovato

        else if(utente_cercato==session){ //Se cerchi il tuo stesso profilo

            My_page();
            uscita=1;
        }

        else{ //Se l'utente esiste

            printf("L'utente e' stato trovato\n");
            uscita=1;
            Apri_pagina (utente_cercato);

        }

    }//Fine ciclo esterno


}

int Controllo_follow(unsigned long long int uid)
{

    Seguiti *scorri=session->amicizie;

    while(scorri!=NULL)
    {
        if ((scorri->uid)==uid){
            return 1; //1=vero se l'uid è stato trovato
        }

        else {
            scorri=scorri->next;
        }
    }

    return 0; //Se esce dal ciclo non è stato trovato alcun uid corrispondente

}

void My_page()
{

    char stringaPost[DIM_POST]; //Eventuale post scritto dall'utente

    Post* scorripost; //scorripost avrà l'indirizzo della lista di post dell'utente che sta utilizzando il profilo
    char scelta; //variabile per la scelta delle diverse funzioni

    clear();
    printf("Stai guardando il tuo profilo, ");
    puts(session->id); //stampa dell'username dell'utente corrente
    putchar('\n');

    do{
        scorripost = session->profilo_utente; //Lo aggiorniamo a ogni iterazione
        if(scorripost==NULL) //Significa che non ci sono post nella pagina utente
        {
            printf("Non hai ancora scritto nessun post, prova a scriverne uno!\n");
        }
        else
        {
            printf("Ecco i tuoi post (dal piu' al meno recente)\n\n\n");
            while(scorripost!=NULL)
            {
                Stampa_post(scorripost->post); //Di volta in volta scorripost andrà a stampare le stringhe (i post) dei nodi
                puts("\n\n");
                scorripost = scorripost->older; //Dopodiché passerà al nodo successivo (il post più vecchio)
            }
        }

        printf("1. Scrivi un post (MAX 1000 caratteri)\n");
        printf("2. Modifica il tuo nome utente\n");
        printf("3. Modifica la tua password\n");
        printf("4. Elimina il tuo account\n");
        printf("0. Esci dalla pagina del tuo profilo\n");
        Input_check(&scelta);

        switch(scelta){

            case '1':

                clear();
                printf("A cosa stai pensando?\n");
                printf("Pensa solo ai contenuti, alla forma ci pensiamo noi!\n");
                printf("Per annullare l'operazione, scrivere \".escape\"\n");

                Post_check(stringaPost);
                if(strcmp(stringaPost,".escape")== 0)
                {
                    printf("Operazione annullata.\n");
                }
                else New_post(stringaPost);

            break;

            case '2':
                clear();
                Modifica_nome();
            break;

            case '3':

                clear();
                Modifica_pw();
            break;

            case '4':

                clear();
                Elimina_account();
            break;

            case '0':

                clear();
            break;

            default:

                clear();
                printf("ERRORE: comando non riconosciuto\n");
            break;

        }

    }while ((scelta!='0')&&(session!=NULL));


}

void Modifica_nome()
{

    char nome_utente[DIM_NOME]; //Stringa per il nome
    char err; //Variabile per controllare il flusso del ciclo
    int uscita; //Fa uscire l'utente dal ciclo
    char scelta; //Variabile utilizzata per l'input dell'utente

    clear();
    do{

        do{

            printf("Inserisci il tuo nuovo nome utente, MAX 20 caratteri (non sono accettati spazi)\n");
            Nome_check(nome_utente); //Si prende il nome tramite nome_check
            err = SpaziOrTabs(nome_utente); //Controllo della presenza di tabs o spazi
            if(strlen(nome_utente)>20 || strlen(nome_utente)==0 ) err = '1';

            if(err == '1')  //Messaggio di errore che compare solo nel caso in cui sono stati inseriti i caratteri non accettati,
                            // si è superata la lunghezza, o inserita la stringa nulla
                printf("ERRORE: assicurarsi di:\n1 Aver rispettato la dimensione massima\n2 Non aver inserito caratteri non accettati (spazi o tab)\n\nRiprovare: \n\n");

        } while (err=='1');

        if (Controllo_Nome(nome_utente)!=NULL){
            //Se il nome inserito è già il nome dell'utente o esiste già

            if(Controllo_Nome(nome_utente)!=session) printf("ERRORE: Il nome inserito e' gia' in uso\n");
            //Se non esiste alcun utente con questo nome

            else printf("ERRORE: Il nome utente e' uguale a quello attuale\n"); //Il nome corrisponde

            do{

                printf("1. Riprova a modificare il tuo nome utente\n");
                printf("0. Torna alla tua pagina\n");

                Input_check(&scelta);

                switch(scelta){

                    case '1':

                        uscita=0;
                    break;

                    case '0':

                        uscita=1;
                    break;

                    default:

                        printf("ERRORE: comando non riconosciuto\n");
                    break;
                }

            }while (scelta<'0'||scelta>'1'); //Ciclo errore

        }

        else{ //Se il nome va bene

            strcpy((session->id), nome_utente);
            printf("La modifica e' avvenuta con successo\n");
            uscita=1;
        }

    } while (uscita==0);

}

void Elimina_account()
{
    Lista_utenti *notpop=current; //notpop sarà graficamente sempre un nodo precedente rispetto a notpopnext, a meno che non bisogna eliminare l'account più recente (in quel caso si farà un pop);
    Lista_utenti *notpopNext=notpop;

    while(notpopNext->uid!=session->uid) //Finche notpopNext non sarà l'user con l'id dell'utente in sessione, si aggiorneranno
    {
        notpop=notpopNext;
        notpopNext=notpopNext->next;
    }
    if(notpop == notpopNext) //Se notpopNext non è mai andato avanti, allora l'uid corrisponde all'utente più recente.
    {
        current = current->next; //Per cui si manda current all'elemento successivo
        Elimina_post();
        Elimina_seguiti;
        free(notpop);
    }
    else
    {
        notpop->next=notpopNext->next; //Altrimenti notpopNext deve essere prima tagliato fuori dalla lista (facendo puntare notpop all'elemento dopo notpopNext)
        Elimina_post();
        Elimina_seguiti();
        free(notpopNext); //E si dealloca lo spazio contenuto in notpopNext
    }
    clear();
    printf("Ci dispiace che tu te ne vada, account eliminato correttamente.\n\n");
    session = NULL; //Si esegue un logout automatico. Le istanze di Mypage() e Session() si chiuderanno, tornando automaticamente in Index()

}

void Elimina_seguiti()
{
    Seguiti *del; //Pointer alla lista dei seguiti di un tente

    while(session->amicizie!=NULL) //Finchè la lista dei seguiti non è nulla
    {
        del=session->amicizie; //del si troverà un nodo prima di session->amicizie per deallocarli man mano dalla memoria
        session->amicizie=session->amicizie->next; //la lista amicizie andrà avanti
        free(del); //Si dealloca il nodo precedente
    }
}

void Elimina_post()
{
    Post *del; //Puntatore per scorrere la lista di post

    while(session->profilo_utente!=NULL) //Finchè la lista dei post non è nulla
    {
        del=session->profilo_utente; //Accede all'ultimo post (Che è quello in testa)
        session->profilo_utente=session->profilo_utente->older; //Reitera  il ciclo
        free(del); //Deallocazione del nodo precedente all'ultimo analizzato
    }
}

void Visualizza_utenti ()
{

    Lista_utenti *scorri_lista=current;

    printf("Ecco i nomi di tutti gli utenti iscritti\n");

    while (scorri_lista!=NULL){

        puts(scorri_lista->id);
        puts("\n");

        scorri_lista=scorri_lista->next;

    }

}

void clear()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void Modifica_pw()
{
    char nuovapw[1000];
    char scelta;
    char uscita;

    do
    {
        do
        {
            printf("1. Inserisci la nuova password\n0. Annulla inserimento nuova password\n");
            Input_check(&scelta);
            switch (scelta)
            {
                case '1':

                    Pw_check(nuovapw);
                    if(strlen(nuovapw)==1 && nuovapw[0]=='0') //Se la stringa è lunga 1 e consta del carattere '0'
                    {
                        scelta = '0';
                    }
                    uscita = '1';
                break;

                case '0':

                    uscita = '1';
                break;

                default:

                    uscita='0';
                    printf("ERRORE: comando non riconosciuto\n");
                break;
            }

        } while(uscita!='1');

        if(scelta == '1') //Se l'utente vuole inserire una password
        {
            if(strcmp(nuovapw,session->pw)==0)//Se la password è uguale a quella già utilizzata
            {
                uscita ='0';
                clear();
                printf("ERRORE: la nuova password e' uguale a quella attuale, riprovare\n");
            }
            else
            {
                clear();
                printf("Modifica avvenuta con successo\n");
                strcpy(session->pw,nuovapw);
            }
        }
        else
        {
            printf("Modifica annullata con successo\n");
        }

    } while (uscita != '1');
}
