#include "controller.h"
/**
  * Passat l'identificador, retorna un string de 6 slots
  * amb l'identificador. p.ex.: 7 -> "000007".
  *
  */
QString IdToString(int id) {
    int len = log10(id);
    len++;
    len = XNUMBER - len;
    QString out;
    for (int i = 0; i < len; ++i)
        out.append("0");

    return out+QString::number(id);
}

/**
  *
  * El Controller és l'encarregat d'executar tota la capa de control de
  * l'aplicació.
  *
  */

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/
    id=1;
    QList<QString> *list = getFilesDirectory(HIST_PATH);
    histograms = (Histogram**)malloc(list->size()*sizeof(Histogram*));
    loadHist(list);
}

/**
  *
  * Carrega un histograma de disc a memòria.
  *
  *
  */

Histogram* getHistogram(int id) {

    Mat hist_h, hist_s, hist_v;
    QString path = QString(HIST_PATH) + "hist_"+IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().data(), FileStorage::READ);

    fs ["hist_h"] >> hist_h;
    fs ["hist_s"] >> hist_s;
    fs ["hist_v"] >> hist_v;

    return new Histogram(hist_h, hist_s, hist_v);
}

/**
  *
  * Passada una imatge et crea l'histograma d'aquella imatge.
  * 
  */

Histogram* createHistogram(QString path) {
    Mat img, hsv_test;
    vector<Mat> hsv_planes;
    img = imread(path.toLatin1().data(), CV_LOAD_IMAGE_COLOR);
    cvtColor(img, hsv_test, CV_BGR2HSV );
    split(hsv_test, hsv_planes);
    /// Bins to use
	int h_bins = 50; int s_bins = 50; int v_bins = 100;

	// Ranges
	float hrang[] = {0, 180};
	const float *h_ranges = { hrang };

	float srang[] = {0, 256};
	const float *s_ranges = { srang };

	float vrang[] = {0, 256};
	const float *v_ranges = { vrang };

	/// Histograms
	Mat hist_h, hist_s, hist_v;

	/// Calculate the histogram for the H image
	calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h, 1, &h_bins, &h_ranges, true, false );
	normalize( hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s, 1, &s_bins, &s_ranges, true, false );
	normalize( hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat() );

	calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v, 1, &v_bins, &v_ranges, true, false );
	normalize( hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat() );

    return new Histogram(hist_h, hist_s, hist_v);
}

/**
  *
  * Guarda un histograma de memòria a disc.
  *
  */

void storeHistogram(int id, Histogram* h) {
    QString path = QString(HIST_PATH) + QString("hist_") + IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().data(), FileStorage::WRITE);

    fs << "hist_h" << *h->hist_h;
    fs << "hist_s" << *h->hist_s;
    fs << "hist_v" << *h->hist_v;

    fs.release();
}

/**
  *
  * Aquesta funció inserta un conjunt d'imatges a la base de dades, 
  * amb els seus histogrames corresponents.
  *
  */

void Controller::insertImages(QList<QString> *list) {

    /*Hem de crear la carpeta 'img/' i 'hist/' si no està creada*/
    DIR *imgdir = opendir(IMG_PATH);
    DIR *histdir = opendir(HIST_PATH);

    if(imgdir == NULL){
        mkdir(IMG_PATH,ACCESSPERMS);
        imgdir = opendir(IMG_PATH);
    }

    if(histdir == NULL){
        mkdir(HIST_PATH,ACCESSPERMS);
        histdir = opendir(HIST_PATH);
    }

    Histogram *h;
    int i;
    int len = list->size();
    histograms = (Histogram**) realloc(histograms, (len+id-1)*sizeof(Histogram*));



    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

    #pragma omp parallel
    {
        #pragma omp for
        for(i = 0; i < len; i++){
            #pragma omp task untied
            {
                int source = open(list->at(i).toStdString().data(), O_RDONLY, 0);
                QString path = IMG_PATH + QString("img_") + IdToString(id+i) + ".jpg";
                int dest = open(path.toStdString().data(), O_WRONLY | O_CREAT, 0644);

                // struct required, rationale: function stat() exists also
                struct stat stat_source;
                fstat(source, &stat_source);

                sendfile(dest, source, 0, stat_source.st_size);

                close(source);
                close(dest);
            }
            #pragma omp taskwait
            {
                #pragma omp task untied
                {
                    // Calcular l'histograma
                    h = createHistogram(list->at(i));
                    histograms[id+i-1] = h;
                    // Guardar l'histograma a 'hist/'
                    storeHistogram(id+i, h);
                }
            }
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);

    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    cout << "Temps d'execució [insertImages]: " << elapsed << " segons" << endl;
    id += len;
}

/**
  *
  * Pasada una imatge, calcula quina de les imatges de la base de dades
  * és més semblant.
  */

QList<QString> *Controller::search(QString path) {
    QList<QString> *out = new QList<QString>();
    int len = id-1;
    int *idx = (int*) malloc(len * sizeof(int));
    double *compares = (double*) malloc(len * sizeof(double));
    Histogram *hist = createHistogram(path);

    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);

	#pragma omp parallel
	{
        #pragma omp single
        {
			for (int i = 0; i < len; ++i) {
				    idx[i] = i+1;
					#pragma omp task untied
				    compares[i] = hist->compare(histograms[i]);
			}
		}
		
		//sort
		quicksort(idx,compares,len);
	}
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    cout << "Temps d'execució [search]: "<< elapsed << " segons" << endl;

    for (int i = 0; i < len; ++i)
        out->append(QString(IMG_PATH) + "img_" + IdToString(idx[i]) + ".jpg");



    return out;

}

/**
  *
  * Carrega tots els histogrames de disc a memòria.
  *
  */

void Controller::loadHist(QList<QString> *list) {
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    int len = list->size();

    #pragma omp parallel for
    for (int i = 1; i <= len; ++i ){
        histograms[i-1] = getHistogram(i);
    }
    id = len +1;

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    cout << "Temps d'execució [loadHist]: "<< elapsed << " segons"<< endl;
}

/**
 * Returns a set of absolute path of files given a directory path.
 *
 * @brief MainWindow::getFilesDirectory
 * @return
 */

QList<QString>* Controller::getFilesDirectory(QString path){
    DIR *dpdf;
    struct dirent *epdf;
    QList<QString> *images = new QList<QString>();
    dpdf = opendir(path.toLatin1().data());
    if (dpdf != NULL){
       while ((epdf = readdir(dpdf))){
          if(epdf->d_name[0] == '.')
              continue;
          images->append(path+"/"+ QString(QLatin1String(epdf->d_name)));

       }
    }
    return images;
}

Controller::~Controller()
{

}

