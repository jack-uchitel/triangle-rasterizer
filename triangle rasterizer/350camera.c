

/* This file offers a camera data type, which encompasses an isometry and a
projection. */


/* Feel free to read from this struct's members, but don't write to them. */
typedef struct camCamera camCamera;
struct camCamera {
    isoIsometry isometry;
    GLdouble projection[6];
    int projectionType;
};



/*** Convenience functions for isometry ***/

/* Sets the camera's isometry, in a manner suitable for third-person viewing.
The camera is aimed at the world coordinates target. The camera itself is
displaced from that target by a distance rho, in the direction specified by the
spherical coordinates phi and theta (as in vec3Spherical). Under normal use,
where 0 < phi < pi, the camera's up-direction is world-up, or as close to it as
possible. */
void camLookAt(
        camCamera *cam, const GLdouble target[3], GLdouble rho, GLdouble phi,
        GLdouble theta) {
    
    /*debug("camLookAt start");*/
    GLdouble z[3], y[3], yStd[3] = {0.0, 1.0, 0.0}, zStd[3] = {0.0, 0.0, 1.0};
    GLdouble rot[3][3], trans[3];
    vec3Spherical(1.0, phi, theta, z);
    vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
    mat33BasisRotation(yStd, zStd, y, z, rot);
    isoSetRotation(&(cam->isometry), rot);
    vecScale(3, rho, z, trans);
    vecAdd(3, target, trans, trans);
    isoSetTranslation(&(cam->isometry), trans);
    /*debug("camLookAt end");*/
}

/* Sets the camera's isometry, in a manner suitable for first-person viewing.
The camera is positioned at the world coordinates position. From that position,
the camera's sight direction is described by the spherical coordinates phi and
theta (as in vec3Spherical). Under normal use, where 0 < phi < pi, the camera's
up-direction is world-up, or as close to it as possible. */
void camLookFrom(
                 
        camCamera *cam, const GLdouble position[3], GLdouble phi, GLdouble theta) {
    GLdouble negZ[3], y[3],  yStd[3] = {0.0, 1.0, 0.0};
    GLdouble negZStd[3] = {0.0, 0.0, -1.0}, rot[3][3];
    vec3Spherical(1.0, phi, theta, negZ);
    vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
    mat33BasisRotation(yStd, negZStd, y, negZ, rot);
    isoSetRotation(&(cam->isometry), rot);
    isoSetTranslation(&(cam->isometry), position);
    
}



/*** Projections ***/

#define camORTHOGRAPHIC 0
#define camPERSPECTIVE 1
#define camPROJL 0
#define camPROJR 1
#define camPROJB 2
#define camPROJT 3
#define camPROJF 4
#define camPROJN 5

/* Sets the projection type, to either camORTHOGRAPHIC or camPERSPECTIVE. */
void camSetProjectionType(camCamera *cam, int projType) {
    /*debug("camSetProjectionTypeStart ");*/
    cam->projectionType = projType;
    /*debug("camSetProjectionTypeEnd ");*/
}

/* Sets all six projection parameters. */
void camSetProjection(camCamera *cam, const GLdouble proj[6]) {
    /*debug("camSetProjectionStart ");*/
    vecCopy(6, proj, cam->projection);
    /*debug("camSetProjectionEnd ");*/
}

/* Sets one of the six projection parameters. */
void camSetOneProjection(camCamera *cam, int i, GLdouble value) {
    cam->projection[i] = value;
}

/* Builds a 4x4 matrix representing orthographic projection with a boxy viewing
volume [left, right] x [bottom, top] x [far, near]. That is, on the near plane
the box is the rectangle R = [left, right] x [bottom, top], and on the far
plane the box is the same rectangle R. Keep in mind that 0 > near > far. Maps
the viewing volume to [-1, 1] x [-1, 1] x [-1, 1], with far going to 1 and near
going to -1. */
void camGetOrthographic(const camCamera *cam, GLdouble proj[4][4]) {
    /*debug("camGetOrthographicStart "); */
    GLdouble l = cam->projection[camPROJL];
    GLdouble r = cam->projection[camPROJR];
    GLdouble b = cam->projection[camPROJB];
    GLdouble t = cam->projection[camPROJT];
    GLdouble f = cam->projection[camPROJF];
    GLdouble n = cam->projection[camPROJN];
    
    proj[0][0] = 2/(r-l);
    proj[0][1] = 0;
    proj[0][2] = 0;
    proj[0][3] = (-r-l)/(r-l);
    
    proj[1][0] = 0;
    proj[1][1] = 2/(t-b);
    proj[1][2] = 0;
    proj[1][3] = (-t-b)/(t-b);
    
    proj[2][0] = 0;
    proj[2][1] = 0;
    proj[2][2] = -2/(n-f);
    proj[2][3] = (n+f)/(n-f);
    
    proj[3][0] = 0;
    proj[3][1] =0;
    proj[3][2] =0;
    proj[3][3] =1;
    /*debug("camGetOrthographicEnd ");*/
    
}

/* Inverse to the matrix produced by camGetOrthographic. */
void camGetInverseOrthographic(const camCamera *cam, GLdouble proj[4][4]) {
    GLdouble left = cam->projection[camPROJL];
    GLdouble right = cam->projection[camPROJR];
    GLdouble bottom = cam->projection[camPROJB];
    GLdouble top = cam->projection[camPROJT];
    GLdouble far = cam->projection[camPROJF];
    GLdouble near = cam->projection[camPROJN];
    mat44Zero(proj);
    proj[0][0] = (right - left) / 2.0;
    proj[0][3] = (right + left) / 2.0;
    proj[1][1] = (top - bottom) / 2.0;
    proj[1][3] = (top + bottom) / 2.0;
    proj[2][2] = (near - far) / -2.0;
    proj[2][3] = (near + far) / 2.0;
    proj[3][3] = 1.0;
}

/* Builds a 4x4 matrix representing perspective projection. The viewing frustum
is contained between the near and far planes, with 0 > near > far. On the near
plane, the frustum is the rectangle R = [left, right] x [bottom, top]. On the
far plane, the frustum is the rectangle (far / near) * R. Maps the viewing
volume to [-1, 1] x [-1, 1] x [-1, 1], with far going to 1 and near going to
-1. */
void camGetPerspective(const camCamera *cam, GLdouble proj[4][4]){
    
    GLdouble l = cam->projection[camPROJL];
    GLdouble r = cam->projection[camPROJR];
    GLdouble b = cam->projection[camPROJB];
    GLdouble t = cam->projection[camPROJT];
    GLdouble f = cam->projection[camPROJF];
    GLdouble n = cam->projection[camPROJN];
    
    proj[0][0]= (-2*n)/(r-l);
    proj[0][1]= 0;
    proj[0][2]= (r+l)/(r-l);
    proj[0][3]= 0;
    
    proj[1][0]= 0;
    proj[1][1]= (-2*n)/(t-b);
    proj[1][2]= (t+b)/(t-b);
    proj[1][3]= 0;
    
    proj[2][0]= 0;
    proj[2][1]= 0;
    proj[2][2]= (n+f)/(n-f);
    proj[2][3]= (-2*n*f)/(n-f);
    
    proj[3][0]= 0;
    proj[3][1]= 0;
    proj[3][2]= -1;
    proj[3][3]= 0;

}




/*** Convenience functions ***/

/* Sets the six projection parameters, based on the width and height of the
viewport and three other parameters. The camera looks down the center of the
viewing volume. For perspective projection, fovy is the full (not half)
vertical angle of the field of vision, in radians. focal > 0 is the distance
from the camera to the focal plane (where 'focus' is used in the sense of
attention, not optics). ratio expresses the far and near clipping planes
relative to focal: far = -focal * ratio and near = -focal / ratio. Reasonable
values are fovy = M_PI / 6.0, focal = 10.0, and ratio = 10.0, so that
far = -100.0 and near = -1.0, but really they depend on the scene being
rendered. For orthographic projection, the projection parameters are set to
produce the orthographic projection that, at the focal plane, is most similar
to the perspective projection just described. You must re-invoke this function
after each time you change the viewport or projection type. */
void camSetFrustum(
        camCamera *cam, GLdouble fovy, GLdouble focal, GLdouble ratio, GLdouble width,
                   GLdouble height) {
    /*debug("camSetFrustumStart ");*/
    cam->projection[camPROJF] = -focal * ratio;
    cam->projection[camPROJN] = -focal / ratio;
    if (cam->projectionType == camPERSPECTIVE)
        cam->projection[camPROJT] = -cam->projection[camPROJN]
            * tan(fovy * 0.5);
    else
        cam->projection[camPROJT] = focal * tan(fovy * 0.5);
    cam->projection[camPROJB] = -cam->projection[camPROJT];
    cam->projection[camPROJR] = cam->projection[camPROJT] * width / height;
    cam->projection[camPROJL] = -cam->projection[camPROJR];
    /*debug("camSetFrustumEnd ");*/
}

/* Returns the homogeneous 4x4 product of the camera's projection and the
camera's inverse isometry. */
void camGetProjectionInverseIsometry(camCamera *cam, GLdouble homog[4][4]) {
    /*debug("camGetProjectionInverseIsometryStart ");*/
    GLdouble camProjection[4][4];
    if (cam->projectionType == 0){
        camGetOrthographic(cam, camProjection);
    }else {
        camGetPerspective(cam, camProjection);
    }
    GLdouble camInverseIsom[4][4];
    isoGetInverseHomogeneous(&(cam->isometry), camInverseIsom);
    mat444Multiply(camProjection,camInverseIsom,homog);
    /*debug("camGetProjectionInverseIsometryEnd ");*/
    
}


/* Inverse to the matrix produced by camGetPerspective. */
void camGetInversePerspective(const camCamera *cam, GLdouble proj[4][4]){
    
    GLdouble l = cam->projection[0];
    GLdouble r = cam->projection[1];
    GLdouble b = cam->projection[2];
    GLdouble t = cam->projection[3];
    GLdouble f = cam->projection[4];
    GLdouble n = cam->projection[5];
    
    proj[0][0]= (r-l)/(-2*n);
    proj[0][1]= 0;
    proj[0][2]= 0;
    proj[0][3]= (r+l)/(-2*n);
    
    proj[1][0]= 0;
    proj[1][1]= (t-b)/(-2*n);
    proj[1][2]= 0;
    proj[1][3]= (t+b)/(-2*n);
    
    proj[2][0]= 0;
    proj[2][1]= 0;
    proj[2][2]= 0;
    proj[2][3]= -1;
    
    proj[3][0]= 0;
    proj[3][1]= 0;
    proj[3][2]= (n-f)/(-2*n*f);
    proj[3][3]=(n+f)/(-2*n*f);
    
}
