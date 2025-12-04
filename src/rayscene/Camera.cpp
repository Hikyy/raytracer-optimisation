#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
#include "Camera.hpp"
#include "../raymath/Ray.hpp"

// OPTIMISATION : Ajout du champ halfHeight pour éviter les divisions répétées dans la boucle de rendu
struct RenderSegment
{
public:
  int rowMin;
  int rowMax;
  Image *image;
  double height;
  double halfHeight; // OPTIMISÉ : Pré-calculé height/2.0 (était calculé 1080 fois par frame !)
  double intervalX;
  double intervalY;
  int reflections;
  Scene *scene;
};

Camera::Camera() : position(Vector3())
{
}

Camera::Camera(Vector3 pos) : position(pos)
{
}

Camera::~Camera()
{
}

Vector3 Camera::getPosition()
{
  return position;
}

void Camera::setPosition(Vector3 &pos)
{
  position = pos;
}

/*
 * OPTIMISATION : Pré-calculer halfHeight pour éviter les divisions répétées
 * 
 * CODE AVANT :
 *   void renderSegment(RenderSegment *segment) {
 *     for (int y = ...) {
 *       double yCoord = (segment->height / 2.0) - (y * segment->intervalY);  // Division à chaque ligne!
 *       ...
 *     }
 *   }
 *   // Pour une image 1920x1080 : 1080 divisions par frame !
 * 
 * CODE APRÈS :
 *   // Calculer UNE SEULE FOIS avant la boucle :
 *   double halfHeight = height * 0.5;  // Multiplication, pas division
 *   segment->halfHeight = halfHeight;
 *   
 *   void renderSegment(RenderSegment *segment) {
 *     for (int y = ...) {
 *       double yCoord = segment->halfHeight - (y * segment->intervalY);  // Utilise valeur pré-calculée
 *       ...
 *     }
 *   }
 *   // Pour une image 1920x1080 : 0 divisions dans la boucle !
 * 
 * Amélioration : Élimine 1080 divisions répétées par frame
 */
void renderSegment(RenderSegment *segment)
{

  for (int y = segment->rowMin; y < segment->rowMax; ++y)
  {
    // CODE AVANT : double yCoord = (segment->height / 2.0) - (y * segment->intervalY);
    double yCoord = segment->halfHeight - (y * segment->intervalY);  // OPTIMISÉ : Utiliser valeur pré-calculée

    for (int x = 0; x < segment->image->width; ++x)
    {
      double xCoord = -0.5 + (x * segment->intervalX);

      Vector3 coord(xCoord, yCoord, 0);
      Vector3 origin(0, 0, -1);
      Ray ray(origin, coord - origin);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

/*
 * OPTIMISATION : Multithreading et allocation sur la pile
 * Changements :
 * 1. Utilisation du multithreading pour diviser l'image en segments
 *    - Détecte automatiquement le nombre de cœurs disponibles
 *    - Crée un segment et un thread par cœur
 *    - Chaque thread rend sa portion de l'image en parallèle
 *    - Flag UseMultithreading pour activer/désactiver le multithreading
 *
 * 2. Allocation sur la pile au lieu du tas (new/delete)
 *    Original : RenderSegment *seg = new RenderSegment();
 *    Optimisé : RenderSegment seg; (allocation pile - pas de surcharge tas)
 *
 * 3. Pré-calculer halfHeight
 *    Stocké dans segment->halfHeight pour éviter division dans boucle serrée
 *
 * Amélioration combinée : Allocation plus rapide + moins de divisions + parallélisation
 */
void Camera::render(Image &image, Scene &scene)
{
  double ratio = (double)image.width / (double)image.height;
  double height = 1.0 / ratio;

  double intervalX = 1.0 / (double)image.width;
  double intervalY = height / (double)image.height;
  double halfHeight = height * 0.5; // OPTIMISÉ : Pré-calculer height/2.0

  scene.prepare();

  // MODE SINGLE-THREAD : Rendu sans multithreading
  if (!this->UseMultithreading)
  {
    RenderSegment seg;
    seg.height = height;
    seg.halfHeight = halfHeight;
    seg.image = &image;
    seg.scene = &scene;
    seg.intervalX = intervalX;
    seg.intervalY = intervalY;
    seg.reflections = Reflections;
    seg.rowMin = 0;
    seg.rowMax = image.height;
    renderSegment(&seg);
    return;
  }

  // MODE MULTITHREADING : Obtenir le nombre de threads disponibles
  unsigned int nthreads = std::thread::hardware_concurrency();
  if (nthreads == 0) nthreads = 1; // Fallback si la détection échoue

  std::vector<std::thread> threads;
  std::vector<RenderSegment> segments(nthreads);

  // OPTIMISATION MULTITHREADING : Diviser l'image en segments
  int rowsPerThread = image.height / nthreads;
  int remainingRows = image.height % nthreads;

  int currentRow = 0;
  for (unsigned int i = 0; i < nthreads; ++i)
  {
    // Créer un segment pour ce thread
    segments[i].height = height;
    segments[i].halfHeight = halfHeight;
    segments[i].image = &image;
    segments[i].scene = &scene;
    segments[i].intervalX = intervalX;
    segments[i].intervalY = intervalY;
    segments[i].reflections = Reflections;
    segments[i].rowMin = currentRow;

    // Distribuer les lignes restantes aux premiers threads
    int rowsForThisThread = rowsPerThread + (i < remainingRows ? 1 : 0);
    segments[i].rowMax = currentRow + rowsForThisThread;
    currentRow += rowsForThisThread;

    // OPTIMISATION MULTITHREADING : Créer un thread pour ce segment
    threads.push_back(std::thread(renderSegment, &segments[i]));
  }

  // OPTIMISATION MULTITHREADING : Attendre que tous les threads se terminent
  for (auto& thread : threads)
  {
    thread.join();
  }
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}
