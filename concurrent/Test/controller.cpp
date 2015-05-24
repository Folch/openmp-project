#include "controller.h"

QString IdToString(int id) {
    int len = log10(id);
    len++;
    len = XNUMBER - len;
    QString out;
    for (int i = 0; i < len; ++i)
        out.append("0");

    return out+QString::number(id);
}

Controller::Controller() {
    /*Cridar a loadHist per carregar els histogrames*/
    id=1;
    QList<QString> *list = getFilesDirectory(HIST_PATH);
    histograms = (Histogram**)malloc(list->size()*sizeof(Histogram*));
    loadHist(list);
}



Histogram* getHistogram(int id) {

    Mat hist_h, hist_s, hist_v;
    QString path = QString(HIST_PATH) + "hist_"+IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().data(), FileStorage::READ);

    fs ["hist_h"] >> hist_h;
    fs ["hist_s"] >> hist_s;
    fs ["hist_v"] >> hist_v;

    return new Histogram(hist_h, hist_s, hist_v);
}


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

void storeHistogram(int id, Histogram* h) {
    QString path = QString(HIST_PATH) + QString("hist_") + IdToString(id) + ".xml";
    FileStorage fs(path.toLatin1().data(), FileStorage::WRITE);

    fs << "hist_h" << *h->hist_h;
    fs << "hist_s" << *h->hist_s;
    fs << "hist_v" << *h->hist_v;

    fs.release();
}

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



    time_t current_time;

    current_time = time(NULL);

    #pragma omp parallel for //private(in,out,buf,read)
    for(i = 0; i < len; i++){

        int source = open(list->at(i).toStdString().data(), O_RDONLY, 0);
        QString path = IMG_PATH + QString("img_") + IdToString(id+i) + ".jpg";
            int dest = open(path.toStdString().data(), O_WRONLY | O_CREAT /*| O_TRUNC/**/, 0644);

            // struct required, rationale: function stat() exists also
            struct stat stat_source;
            fstat(source, &stat_source);

            sendfile(dest, source, 0, stat_source.st_size);

            close(source);
            close(dest);


        /*FILE *in,*out;
        char buf[1024];
        int read;

        in = fopen(list->at(i).toStdString().data(), "rb");
        QString path = IMG_PATH + QString("img_") + IdToString(id+i) + ".jpg";
        out = fopen(path.toStdString().data(), "wb");
        read = 0;
        //  Read data in 1kb chunks and write to output file
        while ((read = fread(buf, 1, 1024, in)) == 1024)
        {
          fwrite(buf, 1, 1024, out);
        }

        //  If there is any data left over write it out
        fwrite(buf, 1, read, out);
        // Copiar la imatge a la carpeta 'img/'
        fclose(out);
        fclose(in);*/
        // Calcular l'histograma
        h = createHistogram(list->at(i));
        histograms[id+i-1] = h;
        // Guardar l'histograma a 'hist/'
        storeHistogram(id+i, h);

    }
    current_time = time(NULL)-current_time;

    cout << "Temps d'execució [insertImages]: " << current_time<< " segons" << endl;
    id += len;
}

QList<QString> *Controller::search(QString path) {
    QList<QString> *out = new QList<QString>();
    int len = id-1;
    int *idx = (int*) malloc(len * sizeof(int));
    double *compares = (double*) malloc(len * sizeof(double));
    Histogram *hist = createHistogram(path);

	clock_t tStart = clock();

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
    cout << "Temps d'execució [search]: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC<< " segons" << endl;

    for (int i = 0; i < len; ++i)
        out->append(QString(IMG_PATH) + "img_" + IdToString(idx[i]) + ".jpg");



    return out;

}

void Controller::loadHist(QList<QString> *list) {
	clock_t tStart = clock();

    for (id = 1; id <= list->size(); ++id) {
        histograms[id-1] = getHistogram(id);
    }

    cout << "Temps d'execució [loadHist]: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << " segons"<< endl;
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

