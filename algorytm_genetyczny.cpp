#include<iostream>
#include<cmath>
#include<cstdlib>
#include<algorithm>
#include<ctime>
#include<fstream>
using namespace std;
const int ile_osob=12,liczba_iteracji=100;
int zad_to_proc[ile_osob][1000];
int tmp[ile_osob][1000];
int next[ile_osob][1000];
struct Osobnik{
    int Cmax=0;
    int indeks=0;
    float funkcja_celu=0;
    float prawd_od=0;
    float prawd_do=0;
    float prawd_dlugosc=0;
};
Osobnik a[ile_osob],b[ile_osob],c[ile_osob];

void heapify(int arr[],int cosjeszcze[], int n, int i)
{
    int largest = i;  // Initialize largest as root
    int l = 2*i + 1;  // left = 2*i + 1
    int r = 2*i + 2;  // right = 2*i + 2
    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;
    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;
    // If largest is not root
    if (largest != i)
    {
        swap(arr[i], arr[largest]);
        swap(cosjeszcze[i], cosjeszcze[largest]);
        // Recursively heapify the affected sub-tree
        heapify(arr,cosjeszcze, n, largest);
    }
}
void heapSort(int arr[],int cosjeszcze[], int n)
{
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr,cosjeszcze, n, i);
    // One by one extract an element from heap
    for (int i=n-1; i>=0; i--)
    {
        // Move current root to end
        swap(arr[0], arr[i]);
        swap(cosjeszcze[0],cosjeszcze[i]);
        // call max heapify on the reduced heap
        heapify(arr,cosjeszcze, i, 0);
    }
}
void generuj(int liczba,int n,int A[])
{
    int i,j;
    switch (liczba)
		{
		case 1: //random
			for (i = 0; i < n; i++)
				{A[i] = rand()%100;cout<<A[i]<<endl;}
			break;

		case 2: //rosnaco
			for (i = 0; i < n; i++)
				{A[i] = i;cout<<A[i]<<endl;}//printf("%d\n", A[i]);
			break;

		case 3: //malejaco
			j = n;
			for (i = 0; i < n; i++)
			{
				j--;A[i] = j;cout<<A[i]<<endl;
			}
			break;

		case 4: //staly
			for (i = 0; i < n; i++)
				{A[i] = 1;cout<<A[i]<<endl;}
			break;

		case 5: //V-ksztaltny
			for (i = 0; i < n / 2; i++)
				{A[i] = n - 2 * i;cout<<A[i]<<endl;}
			for (i = n / 2; i < n; i++)
				{A[i] = (i - n / 2) * 2 + 1;cout<<A[i]<<endl;}
			break;
		default:break;
		}
}

void algorytm_listowy(int m,int n,int pro[],int zad[],int tab[])
{
    int j=0,i=m-1,jakasliczba=0,y,z,c,ink=n-1,*indeks;indeks=new int[m];
    //heapSort(zad,n);//for(y=0;y<n;y++)cout<<zad[y]<<" ";cout<<endl;
    while(j<n)
    {
        if(j<m)
        {
            pro[i]+=zad[ink-j];tab[ink-j]=i;indeks[i]=i;
            i--;//i=i%m;
        }
        else //if(j>=m)
        {
            z=j%m;if(z==0)z=m-1;    //z -> okresla ile porownan
            for(c=z;c>0;c--)
            {
                if(pro[c]<pro[c-1]){swap(pro[c],pro[c-1]);swap(indeks[c],indeks[c-1]);};
            }
            pro[0]+=zad[ink-j];tab[ink-j]=indeks[0];
        }
        j++;
    }
    //for(y=0;y<m;y++)cout<<pro[y]<<" ";cout<<endl;
    delete[]indeks;
}

void algorytm_2(int m,int n,int pro[],int zad[],int tab[])
{
    int j=0,i=m-1,TMPmax,TMPmin,z,c,indmin,jtmp,pos2add=0,noadd=0,ink=n-1,*indeks;indeks=new int[m];
    while(j<n)
    {
        if(j<m)//wypelnienie m procesorow m najwiekszymi zadaniami
        {
            pro[i]+=zad[ink-j];tab[ink-j]=i;indeks[i]=i;
            i--;//i=i%m;
            j++;
        }
        else //if(j>=m)
        {
            TMPmax=TMPmin=pro[m-1];
            z=m-2;noadd=0;
            while(j<n && noadd<m-1)//przydzielam kolejne zadania do procesorow
            {
                jtmp=j;
                while(zad[ink-j]+pro[z]<=TMPmax){pro[z]+=zad[ink-j];tab[ink-j]=indeks[z];j++;}
                if(jtmp==j){
                    noadd++;
                }
                else noadd=0;
                if(z==0)z=m-2;else z--;
            }
            //jesli nie mozna bylo dodac zadania do zadnego procesora
            //to dodaje obecne zadanie do najmniej oblozonego
            if(noadd==m-1)
            {
                //szukam najmniej oblozonego procesora
                for(c=0;c<m-1;c++)
                {
                    if(pro[c]<TMPmin){TMPmin=pro[c];indmin=indeks[c];}
                }
                pro[indmin]+=zad[ink-j];tab[ink-j]=indeks[indmin];j++;
            }
            //sortowanie
            heapSort(pro,indeks,m);
        }
    }
    delete[]indeks;
}

void algorytm_3(int m,int n,int pro[],int zad[],int tab[])
{
    int j=0,i=m-1,TMPmax,TMPmin,z,c,indmin,jtmp,dodane=0,ink=n-1,*indeks;indeks=new int[m];
    int *zadpom;zadpom=new int[n];
    for(j=0;j<n;j++)zadpom[j]=zad[j];
    j=0;
    while(dodane<n)
    {
        if(j<m)//wypelnienie m procesorow m najwiekszymi zadaniami
        {
            pro[i]+=zad[ink-j];tab[ink-j]=i;indeks[i]=i;zadpom[ink-j]=-1;
            i--;//i=i%m;
            j++;dodane++;
        }
        else //if(j>=m)
        {
            TMPmax=TMPmin=pro[m-1];z=m-2;
            while(dodane<n)//przydzielam kolejne zadania do procesorow
            {
                jtmp=n-m-1;
                while(jtmp>=0)
                {
                    if(zadpom[jtmp]!=-1 && zadpom[jtmp]+pro[z]<=TMPmax)
                    {pro[z]+=zad[jtmp];tab[jtmp]=indeks[z];zadpom[jtmp]=-1;dodane++;}
                    jtmp--;
                }
                z--;
                //jesli przeszedlem wszytkie procesory i są zadania do rozdysponowania
                if(z<0 && dodane<n)
                {
                    //ustalam nowy TMPmax -> przydzielam najwieksze zadanie do najmniej obciazonego procesora
                    for(c=0;c<m-1;c++)
                    {if(pro[c]<TMPmin){TMPmin=pro[c];indmin=indeks[c];}}
                    jtmp=n-m-1;
                    while(zadpom[jtmp]==-1 && jtmp>=0)jtmp--; //szukam najwiekszego dostepnego zadania
                    pro[indmin]+=zad[jtmp];zadpom[jtmp]=-1;tab[jtmp]=indeks[indmin];dodane++;
                    //sortowanie
                    z=m-2;
                    heapSort(pro,indeks,m);
                    TMPmax=TMPmin=pro[m-1];
                }
            }
        }
    }
    delete[]zadpom;
    delete[]indeks;
}

bool operator<(Osobnik a,Osobnik b)
{
    return a.Cmax<b.Cmax;
}

int Alicz_Cmax(int zad[],int *proc,int indeks,int n,int m)
{
    int i,Cmax=0;for(i=0;i<m;i++)proc[i]=0;
    for(i=0;i<n;i++)
    {
        proc[(zad_to_proc[indeks][i])]+=zad[i];
    }
    //szukam maksa
    for(i=0;i<m;i++)
    {
        if(proc[i]>Cmax)Cmax=proc[i];
    }
    return Cmax;
}

int Blicz_Cmax(int zad[],int *proc,int indeks,int n,int m)
{
    int i,Cmax=0;for(i=0;i<m;i++)proc[i]=0;
    for(i=0;i<n;i++)
    {
        proc[(tmp[indeks][i])]+=zad[i];
    }
    //szukam maksa
    for(i=0;i<m;i++)
    {
        if(proc[i]>Cmax)Cmax=proc[i];
    }
    return Cmax;
}

void stworz_os(int ile_osob,int sum_of_Cmax,Osobnik a[])
{
    int i;float sumoffcelu=0;
    for(i=0;i<ile_osob;i++)
    {
        a[i].funkcja_celu=sum_of_Cmax-a[i].Cmax;
        sumoffcelu+=a[i].funkcja_celu;
    }
    for(i=0;i<ile_osob;i++)
    {
        a[i].prawd_dlugosc=(a[i].funkcja_celu/sumoffcelu)*100;                   //dla i-tego osobnika
        if(i!=0)
        {
            a[i].prawd_od=a[i-1].prawd_do;
            a[i].prawd_do=a[i].prawd_od+a[i].prawd_dlugosc;
        }
        else
        {
            a[i].prawd_od=0;
            a[i].prawd_do=a[i].prawd_dlugosc;
        }
    }
}

int SUMACMAX(int ile_osob,Osobnik a[])
{
    int suma=0,i;
    for(i=0;i<ile_osob;i++)
    {
        suma+=a[i].Cmax;
    }
    return suma;
}

void generacja_first_osobnikow(int zad[],int ile_osob,int n,int m)//n -liczba zadan, m -liczba procesorow
{
    int sum_of_Cmax=0,i,j,*proc,*ind;proc=new int[m];ind=new int[n];
    for(i=0;i<m;i++)proc[i]=0;
    heapSort(zad,ind,n);
    //chromosom osobnikow z alg listowego
    algorytm_listowy(m,n,proc,zad,ind);
    for(j=0;j<n;j++)zad_to_proc[0][j]=ind[j];
    //algorytm 2
    for(i=0;i<m;i++)proc[i]=0;
    algorytm_2(m,n,proc,zad,ind);
    for(j=0;j<n;j++)zad_to_proc[1][j]=ind[j];

    //algorytm 3
    for(i=0;i<m;i++)proc[i]=0;
    algorytm_3(m,n,proc,zad,ind);
    for(j=0;j<n;j++)zad_to_proc[2][j]=ind[j];

    //chromosomy osobnikow z miare rownym (ilosciowo) podzialem zadan
    for(i=3;i<6 && i<ile_osob;i++)
        {for(j=0;j<n;j++){zad_to_proc[i][j]=(j+i-1)%m;}}
    //chromosomy o losowym przydziale
    for(i=i;i<ile_osob;i++)
    {
        for(j=0;j<n;j++)
        {
            zad_to_proc[i][j]=rand()%m;         //przydzielam losowo zadania do procesorow
        }
    }
    for(i=0;i<ile_osob;i++)
    {
        a[i].Cmax=Alicz_Cmax(zad,proc,i,n,m); //licze Cmax dla i-tego osobnika
        a[i].indeks=i;
        sum_of_Cmax+=a[i].Cmax;
    }
    //genotypy wyswietlon
    //funkcja celu dla metody ruletki
    stworz_os(ile_osob,sum_of_Cmax,a);
    delete[]proc;delete[]ind;
}
void selekcja(int &ile_wybrano,int wybrane[],int zakres,Osobnik a[],int ile_osob)
{
    int i,j,z,los;for(i=0;i<ile_osob;i++)wybrane[i]=-1;
    ile_wybrano=0;
    for(i=0;i<ile_osob;i++)
    {
        los=rand()%zakres;
        //sprawdzam, ktorego elementu dotyczy wylosowana liczba
        j=z=0;bool jest=false;
        while(!(a[j].prawd_od<=los && los<=a[j].prawd_do) && j<ile_osob)j++;
        //sprawdzam,czy dany element ju¿ zostal wybrany
        while(jest==false && wybrane[z]!=-1)
        {
            if(wybrane[z]==j)jest=true;
            else z++;
        }
        if(jest==false){wybrane[z]=j;ile_wybrano++;}
    }
}
void krzyzowanie(int wybrane[],int ile_wybrano,int n,int m,int ile_osob,int zad[])
{
    int p1,p2,i,j,o1,o2; //o1-osobnik pierwszy, o2-osobnik drugi p1-pivot 1, p2 -pivot 2
    for(i=0;i<ile_osob/2;i++)
    {
        o1=wybrane[rand()%ile_wybrano];
        do{o2=wybrane[rand()%ile_wybrano];}while(o1==o2);//rozne
        p1=rand()%(n-1);do{p2=rand()%(n-1);}while(p1==p2);
        if(p1>p2)swap(p1,p2);
        //operacja krzyzowania
        for(j=0;j<n;j++)
        {
            if(j<=p1)
            {
                tmp[2*i][j]=zad_to_proc[o1][j];tmp[2*i+1][j]=zad_to_proc[o2][j];
            }
            else if(j<=p2){
                tmp[2*i][j]=zad_to_proc[o2][j];tmp[2*i+1][j]=zad_to_proc[o1][j];
            }
            else
            {
                tmp[2*i][j]=zad_to_proc[o1][j];tmp[2*i+1][j]=zad_to_proc[o2][j];
            }
        }
        //cout<<"o1: "<<o1<<" o2: "<<o2<<" punkt: "<<punkt_przeciecia<<endl;
    }
    if(ile_osob%2 == 1)//jeszcze jedno dziecko trzeba zrobic, bo ile_osob jest nieparzyste
    {
        o1=wybrane[rand()%ile_wybrano];do{o2=wybrane[rand()%ile_wybrano];}while(o1==o2);//rozne
        p1=rand()%(n-1);do{p2=rand()%(n-1);}while(p1==p2);
        if(p1>p2)swap(p1,p2);
        for(j=0;j<n;j++)
        {
            if(j<=p1)
            {tmp[ile_osob-1][j]=zad_to_proc[o1][j];}
            else if(j<=p2)
            {tmp[ile_osob-1][j]=zad_to_proc[o2][j];}
            else
            {tmp[ile_osob-1][j]=zad_to_proc[o1][j];}
        }
    }
   /* //pokazwyniki_krzyzowania
    for(i=0;i<ile_osob;i++)
    {
        for(j=0;j<n;j++)cout<<zad_to_proc[i][j]<<" ";cout<<"  nowy "<<i<<" ->";
        for(j=0;j<n;j++)cout<<tmp[i][j]<<" ";cout<<endl;
    }*/
}
void wymiana(int ile_osob,int n)
{
    int iA,iB,i,j,*w1,*w2;
    w1=new int[ile_osob];w2=new int[ile_osob];

    for(i=0;i<ile_osob;i++)
    {
        w1[i]=a[i].Cmax;w2[i]=b[i].Cmax;
    }

    for(i=0,iA=0,iB=0;i<ile_osob;i++)                 //wybieram najlepsze osobnik sposrod rodzicow i potomkow
    {
        if(w1[iA] < w2[iB])
        {
                //c[i]=a[iA];
                c[i].Cmax=w1[iA];
                for(j=0;j<n;j++)next[i][j]=zad_to_proc[a[iA].indeks][j];iA++;
        }
        else{
            //c[i]=b[iB];
            c[i].Cmax=w2[iB];
            for(j=0;j<n;j++)next[i][j]=tmp[b[iB].indeks][j];iB++;
        }
        c[i].indeks=i;
    }
    delete[]w1;delete[]w2;
            //najlepsze osobniki to nowi rodzice

    //zad_to_proc=next;
}
void Copy_a_c()
{
    for(int i=0;i<ile_osob;i++)
    {
        a[i].Cmax=c[i].Cmax;
        a[i].indeks=c[i].indeks;
    }
}
void Copy_ztp_next(int n)
{
    int i,j;
    for(i=0;i<ile_osob;i++)
    {
        for(j=0;j<n;j++)zad_to_proc[i][j]=next[i][j];
    }
}

void mutacja(int zad[],int ile_osob,int n,int m,int pr_mut)
{
    int i,j,z,los,value_now,value_new,temp;int *proc;proc=new int[m];
    //pokaz przed mutacja
    for(i=0;i<ile_osob;i++)
    {
        for(j=0;j<n;j++)
        {
            //pokaz
            //if(j==0){for(z=0;z<n;z++)cout<<tmp[i][z]<<" ";}
            los=rand()%101;
            if(los<=pr_mut)
            {
                //wybieramy sposob mutacji/zamiany wartosci
                // 0 -klasyczna mutacja, 1 - cos w stylu wyzazania
                temp=rand()%2;
                if(temp == 0)
                {   value_now=tmp[i][j];
                    do{value_new=rand()%m;}while(value_now==value_new);
                    tmp[i][j]=value_new;
                }
                else
                {
                    temp=rand()%n;
                    swap(tmp[i][j],tmp[i][temp]);
                }
            }
        }
      //  cout<<" "<<i<<"   nowe -> ";
      //  for(z=0;z<n;z++)cout<<tmp[i][z]<<" ";cout<<Blicz_Cmax(zad,proc,i,n,m);cout<<endl;
    }
    //wpisanie danych do Osobnik b
    for(i=0;i<ile_osob;i++)
    {
        b[i].Cmax=Blicz_Cmax(zad,proc,i,n,m); //licze Cmax dla i-tego osobnika
        b[i].indeks=i;
    }
    delete[]proc;
}

void alg_genetyczny(int m,int n,int zad[],int ile_osob,int liczba_iteracji,int &wynik)
{//GENERACJA_PIERWSZYCH_OSOBNIKOW()
 //SELEKCJA_czylilosowanie(); -> KRZYZOWANIE(); -> MUTACJA(); -> WYMIANA_wybieramnajlepsze_z_()

    int *wybrane,i,j,ile_wybrano=0,suma_Cmax=0,prawd_mutacji=10;
    int zakres,poprzedni_najgorszy_Cmax=0,ile_tensam_Cmax=0;
    // informacja do ktorego procesora zostanie przydzielone dane zadanie
    wybrane=new int[ile_osob];
    generacja_first_osobnikow(zad,ile_osob,n,m);
    for(int ri=0;ri<ile_osob;ri++)
    {
        cout<<a[ri].Cmax<<" ";
    }cout<<endl;
    //wlasciwe dzialanie -> warunek stopu to liczba iteracji
    for(i=0;i<liczba_iteracji;i++)
    {
        if(i!=0)
        {
            suma_Cmax=SUMACMAX(ile_osob,a);
            stworz_os(ile_osob,suma_Cmax,a);
        }
        ile_wybrano=0; zakres=101;
        selekcja(ile_wybrano,wybrane,zakres,a,ile_osob);
        //cout<<"ile_wybrano: "<<ile_wybrano<<endl;
        if(ile_wybrano>1)krzyzowanie(wybrane,ile_wybrano,n,m,ile_osob,zad);

        mutacja(zad,ile_osob,n,m,prawd_mutacji);
    //    for(int lol=0;lol<ile_osob;lol++){cout<<lol<<": "<<a[lol].Cmax<<" "<<b[lol].Cmax<<endl;}

        sort(a,a+ile_osob);sort(b,b+ile_osob);

        wymiana(ile_osob,n);
        Copy_a_c();         //a=c
        Copy_ztp_next(n);   //zad_to_proc=next;
        if(i!=0 && poprzedni_najgorszy_Cmax==a[ile_osob-1].Cmax)ile_tensam_Cmax++;
        if(ile_tensam_Cmax==4)
            {ile_tensam_Cmax=0;if(prawd_mutacji<=90)prawd_mutacji+=12;}//jezeli nic sie nie dzieje,to zwiekszam
        if(prawd_mutacji>=2)prawd_mutacji-=2;                           //z kazda iteracja zmniejszam prawd. mutacji
        poprzedni_najgorszy_Cmax=a[ile_osob-1].Cmax;
        for(int ui=0;ui<ile_osob;ui++)
        {
        cout<<a[ui].Cmax<<" ";
        }
        cout<<" --->"<<i<<" p_mut= "<<prawd_mutacji<<endl;
    }
    sort(a,a+ile_osob);
    for(i=0;i<ile_osob;i++)
    {
        cout<<a[i].Cmax<<" ";
    }
    wynik=a[0].Cmax;cout<<endl;

    //zwalnianie pamieci
    delete[]wybrane;
}
int main()
{
    srand(time(0));
    int *pro,*zad,i,n,j,m,Cmax=0,liczba,wejscie;
    //zmienne dla algroytmu genetycznego
    //zad_to_proc=next;

    ifstream p1;string nazwa;bool udalo_sie_wczytac_plik=true;
    cout<<"Read data from a file -> 0, generate data -> 1"<<endl;cin>>wejscie;cout<<endl;
    switch(wejscie)             //wejscie okresla sposob otrzymania danych
    {
        case 0: cout<<"Type the name of file containing data: ";
                cin>>nazwa;cout<<endl;             //wczytujemy dane z pliku
                p1.open(nazwa.c_str());
                if(p1.is_open())                   //jezeli plik jest w katalogu i jego nazwa sie zgadza
                {
                    p1>>m>>n;cout<<"Number of tasks: "<<n<<" , number of processors: "<<m<<endl;
                    pro=new int[m];zad=new int[n];i=0;
                    while(i<n && !p1.eof())
                    {p1>>zad[i];cout<<zad[i++]<<endl;};
                }
                else {
                        udalo_sie_wczytac_plik=false;
                        cout<<"Failed reading the file!"<<endl;
                }
                p1.close();
                break;
        case 1:                     //sami generujemy dane
                cout<<"Type number of tasks: ";cin>>n;cout<<endl;
                cout<<"Type number of processors: ";cin>>m;cout<<endl;    //n- ile zadan     m- ile procesorow
                pro=new int[m];zad=new int[n];                          //implementacja tablic
                cout<<"Type a kind of tasks (from 1 to 5)"<<endl;
                cout<<"1 - random"<<endl;cout<<"2 - ascending"<<endl;
                cout<<"3 - descending"<<endl;cout<<"4 - constant"<<endl;
                cout<<"5 - V-shaped (firstly descending and then ascending)"<<endl;

                cin>>liczba;
                generuj(liczba,n,zad);                                  //generowanie danych w komputerze
                break;
        default:break;
    }
    if((wejscie==0 || wejscie==1) && udalo_sie_wczytac_plik==true)
    {
        for(i=0;i<m;i++)pro[i]=0; //przygotowuje tablice procesorow
        alg_genetyczny(m,n,zad,ile_osob,liczba_iteracji,Cmax);    //wykonywanie algorytmu
        //zwracamy wyniki

        cout<<"Cmax for genetic algorithm = "<<Cmax<<endl;cout<<"---------------------------------"<<endl;

        for(i=0;i<m;i++)pro[i]=0; //przygotowuje tablice procesorow
        int *costam=new int[n];
        algorytm_listowy(m,n,pro,zad,costam);Cmax=0;
        //zwracamy wyniki
        for(i=0;i<m;i++)
        {   //cout<<i<<"-ty procesor: "<<pro[i]<<endl;
            if(pro[i]>Cmax)Cmax=pro[i];
        }
        cout<<"Cmax for list scheduling algorithm = "<<Cmax<<endl;

        delete[]costam;
        delete[]pro;
        delete[]zad;
    }
    //podaj cokolwiek aby wyjsc z programu
    system("pause");
    return 0;
}
