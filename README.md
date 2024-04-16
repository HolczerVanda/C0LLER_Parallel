# **Féléves beadandó feladat**

## **LU felbontás N dimenziós mátrixok esetén**

<u>**Futtatás**</u>
A program futtatásához szükséges az OpenCL SDK, valamint a Python legújabb verziója amelynek tartalmaznia kell a `pyopencl`, illetve a `matplotlib`  package-eket. 

<u>**Feladat leírása**</u>
A féléves beadandó feladatom témája különböző méretű mátrixok LU felbontása. Az LU felbontás egy alsó(L-Lower)- és egy felső(U-Upper) háromszög mátrix szorzatára bontja a mátrixot. Az alsó háromszög mátrix, olyan mátrix, melynek a főátlója feletti összes eleme nulla, a felső pedig olyan, melynek a főátlója alatti összes eleme nulla.

<u>**Közös részek**</u>
 A beadandó alapvetően két fajta kóddal rendelkezik, az egyik a szekvenciális módon működő Python kód, a másik pedig a PyOpenCL program. Ebben a részben összefoglalom azokat a közös elemeket, amelyek mindkét kódban megtalálhatók:
 - **Véletlenszámokkal feltöltött mátrix generálása**:
	 - Mindkét kód generál egy *N*×*N* méretű mátrixot, ahol *N* a mátrix dimenzió száma.
	 - A mátrix elemei véletlenszerűen generált egész számok 1-től 10-ig.
 - **LU felbontás algoritmusa**:
	 -   Mindkét kód implementálja az *LU felbontás* algoritmusát, ami egy négyzetes mátrixot, például *A*, felbont két háromszögmátrixra, egy alsó háromszögmátrixra (L) és egy felső háromszögmátrixra (U).
	-   Az algoritmus lépései közé tartozik a *Gauss-elimináció* és az *alsó- és felső háromszögmátrixok kiszámítása*.
- **Grafikon rajzolása**:
	-   Mindkét kód a `matplotlib` könyvtárat használja a grafikonok rajzolásához.
	-   A grafikonokon a futási idő (Execution Time) jelenik meg a Mátrix méretének (Matrix Size) függvényében.
	- Az X tengelyen a dimenziószám (N), az Y tengelyen pedig a futási idő (seconds) jelenik meg másodpercekben.
	-   A kódok a `plt.plot()` függvény segítségével jelenítik meg a pontokat az **(N,végrehajtási idő)** koordinátákon.

<u>**Szekvenciális Python program**</u>
 - **LU felbontás algoritmusa**:
	- Az LU felbontás az `luDecomposition` függvényben történik
    - Az *N* változóban eltárolja az *A* input mátrix méretét, az *L* és *U* mátrixokat pedig létrehozza (azonos dimenziószámmal) és feltölti nullákkal.
    - Ezután történik az *L* és az *U* mátrixok elemeinek kiszámítása az algoritmus végrehajtásával, majd a mátrixok visszaadása
- **Futási idő mérése és grafikon rajzolása**:
    -   A kód a `sizes` változóban tárolja az *N* mátrix méreteket, amelyeken tesztelni fog.
    -   Minden *N* méretre generál egy véletlenszerű *N*×*N* méretű mátrixot.
    -   Ezután leméri az algoritmus végrehajtási idejét az adott méretű mátrixra.
    -   A futási időket egy listában tárolja, majd kirajzolja a futási időt az *N* méretek függvényében.
- **Eltérések az *OpenCL* kódhoz képest**:
    -   A kód szekvenciálisan, egy szálon fut.
    -   Egyszerűbben megvalósítható, de kevésbé hatékony nagyobb méretű mátrixok esetén, ezért az input méretek 100-tól 500-ig vannak megadva, 50-es lépésekkel.
- **Grafikon**:
![image](https://github.com/HolczerVanda/C0LLER_Parallel/assets/90444495/a2009b07-86e3-42a8-8120-7df361d4479d)

<u>**PyOpenCL változat:**</u>
- **Környezet és folyamatok:** 
    -   A kód `PyOpenCL`-t használ a párhuzamosított számításokhoz.
    -   A `PyOpenCL` egy *Python* könyvtár, amely lehetővé teszi `OpenCL` (*Open Computing Language*) programok írását *Pythonba*n.
    -   Az `OpenCL` egy nyílt szabványú keretrendszer, amely lehetővé teszi a párhuzamos számítások elvégzését különböző hardverplatformokon, például CPU-kon és GPU-kon.
-  **Kernel fájl használata:** 
    -   A kódban található egy `kernel.cl` fájl, amelyben a kernelek vannak definiálva.
    -   A kernelek a párhuzamosított számításokat végzik el az `OpenCL` által támogatott hardveren (jelen esetben a *GPU*-n).
    -   A `kernel.cl` fájlt a kód a `PyOpenCL` segítségével olvassa be és használja.
- **Párhuzamos számítások:**
    -   A kód párhuzamosítva fut, amely lehetővé teszi a nagyobb méretű mátrixok hatékonyabb kezelését.
    -   A `luDecomposition` függvény párhuzamosan végzi el az LU felbontást az `OpenCL` kernel segítségével.
- **Futási idő mérése és grafikon rajzolása:**    
    -   A kód ugyanazt a módszert használja a futási idő mérésére és a grafikonok rajzolására, mint a szekvenciális *Python* változat.
    -   A különbség az, hogy a számítások párhuzamosan történnek, ezáltal rövidülnek a futási idők.
- **Különbségek a szekvenciális Python változathoz képest:**
	-   Az `OpenCL`-t használó változat nagyobb teljesítményt nyújt, de nehezebben érthető és implementálható, mint a szekvenciális változat.
	-   A kód bonyolultabb, mivel az `OpenCL` nyelvét és struktúráit használja a párhuzamos számítások kezelésére.
  -   A `PyOpenCL` változat hatékonyabban kezeli a nagyobb méretű mátrixokat, mivel a számítások párhuzamosan történnek, ezért az input méretek 600-tól 2000-ig vannak megadva, 200-as lépésekkel.
- **Grafikon**:
![image](https://github.com/HolczerVanda/C0LLER_Parallel/assets/90444495/31239ccf-f7be-4657-92eb-0f88fc217fa2)
