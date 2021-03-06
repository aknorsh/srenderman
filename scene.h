#ifndef SCENE_H_
#define SCENE_H_
#include <random>
#include "hitable_list.h"
#include "primitive.h"
#include "material.h"
#include "texture.h"
#include "bvh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


hitable *random_scene() {
    int n=5000;
    hitable **list = new hitable*[n+1];
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    list[0] = new sphere(vec3(0,-1000,0),1000,new lambertian(checker));
    int i=1;
    for(int a=-11;a<11;a++){
        for(int b=-11;b<11;b++){
            float choose_mat = drand();
            vec3 center(a+0.9*drand(),0.2,b+0.9*drand());
            if((center-vec3(4,0.2,0)).length() > 0.9){
                if(choose_mat < 0.8) { //diffuse
                    list[i++] = new moving_sphere(center, center + vec3(0.5 * drand(), 0, 0.5 * drand()), 0.0, 1.0, 0.2, new lambertian(new constant_texture(vec3(drand(), drand(), drand()))));
                }
                else if(choose_mat < 0.95){ //metal
                    list[i++] = new sphere(center, 0.2, new metal(vec3(
                        0.5*(1+drand()), 0.5*(1+drand()), 0.5*(1+drand())),
                        0.5*drand()));
                }
                else {
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5)); 
                }
            }
        }
    }
    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(new constant_texture(vec3(0.4, 0.2, 0.1))));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
    return new bvh_node(list,i, 0.0, 1.0);
}

hitable *two_sphere() {
    texture *checker = new checker_texture(new constant_texture(vec3(0.2, 0.3, 0.1)), new constant_texture(vec3(0.9, 0.9, 0.9)));
    int n = 50;
    hitable **list = new hitable*[n+1];
    list[0] = new sphere(vec3(0, -10, 0), 10, new lambertian(checker));
    list[1] = new sphere(vec3(0, 10, 0), 10, new lambertian(checker));

    return new hitable_list(list, 2);
}

hitable *two_perlin_spheres() {
    texture *pertext = new noise_texture(5);
    hitable **list = new hitable*[2];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0, 2, 0), 2, new lambertian(pertext));
    return new hitable_list(list, 2);
}

hitable *earth_with_metal() {
    int nx, ny, nn;
    unsigned char * tex_data = stbi_load("4096px-Nasa_land_ocean_ice_8192.jpg", &nx, &ny, &nn, 0);
    material *mat = new lambertian(new image_texture(tex_data, nx, ny));
    hitable **list = new hitable*[7];
    list[0] = new sphere(vec3(-8, 0, 0), 2, new metal(vec3(0.5, 0.5, 0.5), 0));
    list[1] = new sphere(vec3(8, 0, 0), 2, new metal(vec3(0.5, 0.5, 0.5), 0));
    list[2] = new sphere(vec3(0, 10, 0), 2, new metal(vec3(0.5, 0.5, 0.5), 0));
    list[3] = new sphere(vec3(0, 0, -8), 2, new metal(vec3(0.5, 0.5, 0.5), 0));
    list[4] = new sphere(vec3(0, 0, 8), 2, new metal(vec3(0.5, 0.5, 0.5), 0));

    list[5] = new sphere(vec3(0, 3, 0), 3, mat);
    list[6] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(new constant_texture(vec3(0.8, 0.8, 0.8))));
    return new hitable_list(list, 7);
}

hitable * simple_light() {
    texture *pertext = new noise_texture(4);
    hitable **list = new hitable*[4];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(pertext));
    list[1] = new sphere(vec3(0,2,0), 2, new lambertian(pertext));
    list[2] = new sphere(vec3(0,7,0), 2, new diffuse_light(new constant_texture(vec3(4,4,4))));
    list[3] = new xy_rect(3, 5, 1, 3, -2, new diffuse_light(new constant_texture(vec3(4, 4, 4))));
    return new hitable_list(list, 4);
}

hitable * cornell_box() {
    hitable **list = new hitable *[8];
    int i = 0;
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new zx_rect(127, 432, 113, 443, 400, light);
    list[i++] = new flip_normals(new zx_rect(0, 555, 0, 555, 555, white));
    list[i++] = new zx_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
    return new hitable_list(list, i);
}

hitable *cornell_smoke() {
    hitable **list = new hitable*[8];
    int i = 0;

    texture *red_tx = new constant_texture(vec3(0.65, 0.05, 0.05));
    texture *white_tx = new constant_texture(vec3(0.73, 0.73, 0.73));
    texture *green_tx = new constant_texture(vec3(0.12, 0.45, 0.15));
    texture *light_tx = new constant_texture(vec3(7, 7, 7));

    material *red = new lambertian(red_tx);
    material *white = new lambertian(white_tx);
    material *green = new lambertian(green_tx);
    material *light = new diffuse_light(light_tx);

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new zx_rect(127, 432, 113, 443, 554, light);
    list[i++] = new flip_normals(new zx_rect(0, 555, 0, 555, 555, white));
    list[i++] = new zx_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    hitable *b1 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 165, 165), white), -18), vec3(130, 0, 65));
    hitable *b2 = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));
    list[i++] = new constant_medium(b1, 0.01, new constant_texture(vec3(1.0, 1.0, 1.0)));
    list[i++] = new constant_medium(b2, 1, new constant_texture(vec3(0.0, 0.0, 0.0)));
    return new hitable_list(list, i);
}

void final(hitable **scene, camera **cam, float aspect, hitable_list **hlist) {
    int nb = 20;
    hitable **list = new hitable*[30];
    hitable **boxlist = new hitable*[10000];
    hitable **boxlist2 = new hitable*[10000];
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *ground = new lambertian(new constant_texture(vec3(0.48, 0.48, 0.48)));

    // boxes on ground
    int b = 0;
    for (int i = 0; i < nb; i++) {
        for(int j=0;j<nb;j++) {
            float w = 100;
            float x0 = -1000 + i*w;
            float z0 = -1000 + j*w;
            float y0 = 0;
            float x1 = x0 + w;
            float y1 = 100 * (drand() + 0.01);
            float z1 = z0 + w;
            boxlist[b++] = new box(vec3(x0, y0, z0), vec3(x1, y1, z1), ground);
        }
    }

    int l = 0;
    list[l++] = new bvh_node(boxlist, b, 0, 1);
    material *light = new diffuse_light(new constant_texture(vec3(7, 7, 7)));
    list[l++] = new flip_normals(new zx_rect(147, 412, 123, 423, 554, light));
    vec3 center(400, 400, 200);
    list[l++] = new moving_sphere(center, center + vec3(30, 0, 0), 0, 1, 50, new lambertian(new constant_texture(vec3(0.7, 0.3, 0.1))));
    list[l++] = new sphere(vec3(260, 150, 45), 50, new dielectric(1.5));
    list[l++] = new sphere(vec3(0,150,145), 50, new metal(vec3(0.8,0.8,0.9), 10.0));

    hitable *boundary = new sphere(vec3(360, 150, 145), 70, new dielectric(1.5));
    list[l++] = boundary;
    list[l++] = new constant_medium(boundary, 0.2, new constant_texture(vec3(0.2, 0.4, 0.9)));
    boundary = new sphere(vec3(0, 0, 0), 5000, new dielectric(1.5));
    list[l++] = new constant_medium(boundary, 0.0001, new constant_texture(vec3(1.0, 1.0, 1.0)));

    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("4096px-Nasa_land_ocean_ice_8192.jpg", &nx, &ny, &nn, 0);
    material *emat = new lambertian(new image_texture(tex_data, nx, ny));
    list[l++] = new sphere(vec3(400,200,400), 100, emat);
    texture *pertext = new noise_texture(0.1);
    list[l++] = new sphere(vec3(220, 280, 300), 80, new lambertian(pertext));
    int ns = 1000;
    for(int j=0;j<ns;j++){
        boxlist2[j] = new sphere(vec3(165 * drand(), 165 * drand(), 165 * drand()), 10, white);
    }
    list[l++] = new translate(new rotate_y(new bvh_node(boxlist2, ns, 0.0, 1.0), 15), vec3(-100, 270, 395));

    *scene = new hitable_list(list, l);

    vec3 lookfrom(350, 150, -500);
    vec3 lookat(200, 300, 300);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    *cam = new camera(lookfrom, lookat, vec3(0,1,0), vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);

    hitable **a = new hitable *[1];
    a[0] = new zx_rect(147, 412, 123, 423, 554, 0); // light_shape
    *hlist = new hitable_list(a, 1);
}

void cornell_box(hitable **scene, camera **cam, float aspect, hitable_list **hlist)
{
    int i = 0;
    hitable ** list = new hitable*[8];
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));
    material *light = new diffuse_light(new constant_texture(vec3(15, 15, 15)));

    list[i++] = new flip_normals(new yz_rect(0, 555, 0, 555, 555, green));
    list[i++] = new yz_rect(0, 555, 0, 555, 0, red);
    list[i++] = new flip_normals(new zx_rect(227, 332, 213, 343, 554, light));
    list[i++] = new flip_normals(new zx_rect(0, 555, 0, 555, 555, white));
    list[i++] = new zx_rect(0, 555, 0, 555, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 555, 0, 555, 555, white));

    material *glass = new dielectric(1.5);
    list[i++] = new sphere(vec3(190, 90, 190), 90, glass);

    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

    *scene = new hitable_list(list, i);

    vec3 lookfrom(278, 278, -800);
    vec3 lookat (278,278,0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    *cam = new camera(lookfrom, lookat, vec3(0,1,0), vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);

    hitable **a = new hitable *[2];
    a[0] = new zx_rect(227, 332, 213, 343, 554, 0); // light_shape
    a[1] = new sphere(vec3(190, 90, 190), 90, 0); // glass_shape
    *hlist = new hitable_list(a, 2);
}

void GI(hitable **scene, camera **cam, float aspect, hitable_list **hlist) {

    material *white = new lambertian(new constant_texture(vec3(0.73, 0.73, 0.73)));
    material *red = new lambertian(new constant_texture(vec3(0.65, 0.05, 0.05)));
    material *green = new lambertian(new constant_texture(vec3(0.12, 0.45, 0.15)));

    material *light = new diffuse_light(new constant_texture(vec3(5, 5, 5)));
    material *glass = new dielectric(1.5);
    material *void_mat = new diffuse_light(new constant_texture(vec3(0, 0, 0)));
    int nx, ny, nn;
    unsigned char *tex_data = stbi_load("japanese-wall.jpg", &nx, &ny, &nn, 0);
    material *emat = new lambertian(new image_texture(tex_data, nx, ny));

    // scene
    int i = 0;
    hitable ** list = new hitable*[22];

    // mainbox
    list[i++] = new flip_normals(new yz_rect(0, 500, 0, 300, 500, green));
    list[i++] = new flip_normals(new yz_rect(0, 300, 300, 500, 500, green));
    list[i++] = new yz_rect(200, 500, 0, 200, 0, red);
    list[i++] = new yz_rect(0, 500, 200, 500, 0, red);
    list[i++] = new flip_normals(new zx_rect(0, 500, 0, 500, 500, white));
    list[i++] = new zx_rect(0, 500, 0, 500, 0, white);
    list[i++] = new flip_normals(new xy_rect(0, 500, 0, 500, 500, emat));
    // void mask
    list[i++] = new flip_normals(new xy_rect(-100, 0, 0, 500, 0, void_mat));
    list[i++] = new flip_normals(new xy_rect(500, 1000, 0, 1000, 0, void_mat));

    // subbox(left-up)
    list[i++] = new flip_normals(new yz_rect(300, 500, 300, 500, 550, white));
    list[i++] = new flip_normals(new xy_rect(500, 550, 300, 500, 500, white));
    list[i++] = new xy_rect(500, 550, 300, 500, 300, white);
    list[i++] = new flip_normals(new zx_rect(300, 500, 500, 550, 500, white));
    list[i++] = new zx_rect(300, 500, 500, 550, 300, white);
    // light
    list[i++] = new flip_normals(new yz_rect(320, 480, 320, 480, 549, light));

    // subbox(right-down)
    list[i++] = new yz_rect(0, 200, 0, 200, -100, white);
    list[i++] = new flip_normals(new xy_rect(-100, 0, 0, 200, 200, white));
    list[i++] = new xy_rect(-100, 0, 0, 200, 0, white);
    list[i++] = new flip_normals(new zx_rect(0, 200, -100, 0, 200, white));
    list[i++] = new zx_rect(0, 200, -100, 0, 0, white);
    //light
    list[i++] = new yz_rect(30, 170, 30, 170, -99, light);

    // object
    list[i++] = new sphere(vec3(190, 90, 190), 90, glass);
    list[i++] = new translate(new rotate_y(new box(vec3(0, 0, 0), vec3(165, 330, 165), white), 15), vec3(265, 0, 295));

    *scene = new hitable_list(list, i);

    // camera
    vec3 lookfrom(250, 250, -800);
    vec3 lookat (250, 250, 0);
    float dist_to_focus = 10.0;
    float aperture = 0.0;
    float vfov = 40.0;
    *cam = new camera(lookfrom, lookat, vec3(0,1,0), vfov, aspect, aperture, dist_to_focus, 0.0, 1.0);

    // light
    hitable **a = new hitable *[3];
    a[0] = new yz_rect(320, 480, 320, 480, 549, 0);
    a[1] = new yz_rect(30, 170, 30, 170, -99, 0);
    a[2] = new sphere(vec3(190, 90, 190), 90, 0);
    *hlist = new hitable_list(a, 3);
}

#endif