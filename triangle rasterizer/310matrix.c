/*** 2 x 2 Matrices ***/

/* Pretty-prints the given matrix, with one line of text per row of matrix. */
void mat22Print(const GLdouble m[2][2]) {
    int i, j;
    for (i = 0; i < 2; i += 1) {
        for (j = 0; j < 2; j += 1)
            printf("%f    ", m[i][j]);
        printf("\n");
    }
}

/* Returns the determinant of the matrix m. If the determinant is 0.0, then the
matrix is not invertible, and mInv is untouched. If the determinant is not 0.0,
then the matrix is invertible, and its inverse is placed into mInv. The output
CANNOT safely alias the input. */
GLdouble mat22Invert(const GLdouble m[2][2], GLdouble mInv[2][2]) {
    GLdouble det= (m[0][0]*m[1][1])-(m[0][1]*m[1][0]);
    if (det != 0.0){
        GLdouble f = 1/det;
        mInv[0][0]= m[1][1] *f;
        mInv[0][1]= -1* m[0][1] * f;
        mInv[1][0]= -1 *m[1][0] * f;
        mInv[1][1] = m[0][0] *f;
    }
    return det;
}

/* Multiplies a 2x2 matrix m by a 2-column v, storing the result in mTimesV.
The output CANNOT safely alias the input. */
void mat221Multiply(const GLdouble m[2][2], const GLdouble v[2],
        GLdouble mTimesV[2]) {
    
    mTimesV[0]= (m[0][0]*v[0])+(m[0][1]*v[1]);
    mTimesV[1]= (m[1][0]*v[0])+(m[1][1]*v[1]);
}

/* Fills the matrix m from its two columns. The output CANNOT safely alias the
input. */
void mat22Columns(const GLdouble col0[2], const GLdouble col1[2], GLdouble m[2][2]) {
    m[0][0]=col0[0];
    m[1][0]=col0[1];
    m[0][1]=col1[0];
    m[1][1]=col1[1];
}

/* The theta parameter is an angle in radians. Sets the matrix m to the
rotation matrix corresponding to counterclockwise rotation of the plane through
the angle theta. */
void mat22Rotation(GLdouble theta, GLdouble m[2][2]) {
    m[0][0] = cos(theta);
    m[0][1] = -1* sin(theta);
    m[1][0] = sin(theta);
    m[1][1] = cos(theta);
}


/* Jade Kandel and Jack Uchitel*/

void mat333Multiply(const GLdouble m[3][3], const GLdouble n[3][3], GLdouble mTimesN[3][3]) {
    mTimesN[0][0] = (m[0][0]*n[0][0])+(m[0][1]*n[1][0])+(m[0][2]*n[2][0]);
    mTimesN[0][1] = (m[0][0]*n[0][1])+(m[0][1]*n[1][1])+(m[0][2]*n[2][1]);
    mTimesN[0][2] = (m[0][0]*n[0][2])+(m[0][1]*n[1][2])+(m[0][2]*n[2][2]);
    
    mTimesN[1][0] = (m[1][0]*n[0][0])+(m[1][1]*n[1][0])+(m[1][2]*n[2][0]);
    mTimesN[1][1] = (m[1][0]*n[0][1])+(m[1][1]*n[1][1])+(m[1][2]*n[2][1]);
    mTimesN[1][2] = (m[1][0]*n[0][2])+(m[1][1]*n[1][2])+(m[1][2]*n[2][2]);
    
    mTimesN[2][0] = (m[2][0]*n[0][0])+(m[2][1]*n[1][0])+(m[2][2]*n[2][0]);
    mTimesN[2][1] = (m[2][0]*n[0][1])+(m[2][1]*n[1][1])+(m[2][2]*n[2][1]);
    mTimesN[2][2] = (m[2][0]*n[0][2])+(m[2][1]*n[1][2])+(m[2][2]*n[2][2]);
    
    
}

void mat331Multiply(const GLdouble m[3][3], const GLdouble v[3], GLdouble mTimesV[3]){
    mTimesV[0] = (m[0][0]*v[0])+(m[0][1]*v[1])+(m[0][2]*v[2]);
    mTimesV[1] = (m[1][0]*v[0])+(m[1][1]*v[1])+(m[1][2]*v[2]);
    mTimesV[2] = (m[2][0]*v[0])+(m[2][1]*v[1])+(m[2][2]*v[2]);
    
    
}

void mat33Isometry(GLdouble theta, const GLdouble t[2], GLdouble isom[3][3]) {
    isom[0][0] = cos(theta);
    isom[0][1] = -sin(theta);
    isom[0][2] = t[0];
    
    isom[1][0] = sin(theta);
    isom[1][1] = cos(theta);
    isom[1][2] = t[1];
    
    isom[2][0] = 0;
    isom[2][1] = 0;
    isom[2][2] = 1;
    
}

void mat33Add(const GLdouble m[3][3], const GLdouble n[3][3], GLdouble mTimesN[3][3]){
    int i;
    int j;
    for (i=0; i<3; i++){
        for (j=0; j<3; j++){
            mTimesN[i][j] = m[i][j]+n[i][j];
        }
    }
}

void mat33Scale(GLdouble c, GLdouble m[3][3], GLdouble mTimesc[3][3] ){
    int i;
    int j;
    for (i=0; i<3; i++){
        for (j=0; j<3; j++){
            mTimesc[i][j] = m[i][j]*c;
        }
    }
}

void mTranspose(GLdouble m[3][3], GLdouble mTranspose[3][3]){
    int i;
    int j;
    for (i=0; i<3; i++){
        for (j=0; j<3; j++){
            mTranspose[i][j] = m[j][i];
        }
    }
}

void mTranspose4(GLdouble m[4][4], GLdouble mTranspose[4][4]){
    int i;
    int j;
    for (i=0; i<4; i++){
        for (j=0; j<4; j++){
            mTranspose[i][j] = m[j][i];
        }
    }
}


/* Given a length-1 3D vector axis and an angle theta (in radians), builds the
rotation matrix for the rotation about that axis through that angle. */
void mat33AngleAxisRotation(
                            GLdouble theta, const GLdouble axis[3], GLdouble rot[3][3]){
    GLdouble U[3][3] = { {0,-axis[2], axis[1]}, {axis[2], 0 , -axis[0]}, {-axis[1], axis[0],0}};
    GLdouble Usquared[3][3];
    mat333Multiply(U,U,Usquared);
    GLdouble identityMatrix[3][3] = {{1,0,0}, {0,1,0}, {0,0,1}};
    GLdouble sum1[3][3];
    GLdouble product1[3][3];
    GLdouble product2 [3][3];
    mat33Scale(sin(theta), U, product1);
    mat33Scale((1-cos(theta)), Usquared, product2);
    mat33Add(identityMatrix, product1, sum1);
    mat33Add(sum1, product2, rot);

    
}

/* Given two length-1 3D vectors u, v that are perpendicular to each other.
Given two length-1 3D vectors a, b that are perpendicular to each other. Builds
the rotation matrix that rotates u to a and v to b. */
void mat33BasisRotation(
        const GLdouble u[3], const GLdouble v[3], const GLdouble a[3],
                        const GLdouble b[3], GLdouble rot[3][3]) {
    /* 2 cross products: axb and uxv . make 2 matrices 3x3 each with columns u v uxv and a b axb . compute M = S times transpose R*/
    GLdouble aCb[3];
    GLdouble uCv[3];
    vec3Cross( a,b,aCb );
    vec3Cross( u,v,uCv );
    GLdouble S[3][3] = {{a[0], b[0], aCb[0]}, {a[1], b[1], aCb[1]}, {a[2], b[2], aCb[2]}};
    GLdouble R[3][3] = {{u[0], v[0], uCv[0]}, {u[1], v[1], uCv[1]}, {u[2], v[2], uCv[2]}};
    GLdouble rTranspose[3][3];
    mTranspose(R, rTranspose);
    mat333Multiply(S,rTranspose,rot);
    
}

/* Multiplies m by n, placing the answer in mTimesN. The output CANNOT safely
alias the input. */
void mat444Multiply(
                    const GLdouble m[4][4], const GLdouble n[4][4], GLdouble mTimesN[4][4]) {
    int i;
    int j;
    for (i=0; i<4; i++){
        for (j=0; j<4;j++){
            mTimesN[i][j] = m[i][0]*n[0][j] + m[i][1]*n[1][j] + m[i][2]*n[2][j] + m[i][3]*n[3][j];
        }
    }
    
}

void mat444MultiplyDouble(
                    const double m[4][4], const double n[4][4], double mTimesN[4][4]) {
    int i;
    int j;
    for (i=0; i<4; i++){
        for (j=0; j<4;j++){
            mTimesN[i][j] = m[i][0]*n[0][j] + m[i][1]*n[1][j] + m[i][2]*n[2][j] + m[i][3]*n[3][j];
        }
    }
    
}

/* Multiplies m by v, placing the answer in mTimesV. The output CANNOT safely
alias the input. */
void mat441Multiply(
                    const GLdouble m[4][4], const GLdouble v[4], GLdouble mTimesV[4]) {
    mTimesV[0] = m[0][0]*v[0] + m[0][1]*v[1] + m[0][2]*v[2] + m[0][3]*v[3];
    mTimesV[1] = m[1][0]*v[0] + m[1][1]*v[1] + m[1][2]*v[2] + m[1][3]*v[3];
    mTimesV[2] = m[2][0]*v[0] + m[2][1]*v[1] + m[2][2]*v[2] + m[2][3]*v[3];
    mTimesV[3] = m[3][0]*v[0] + m[3][1]*v[1] + m[3][2]*v[2] + m[3][3]*v[3];
}

/* Given a rotation and a translation, forms the 4x4 homogeneous matrix
representing the rotation followed in time by the translation. */
void mat44Isometry(
                   const GLdouble rot[3][3], const GLdouble trans[3], GLdouble isom[4][4]){
   
    isom[0][0] = rot[0][0];
    isom[0][1] = rot[0][1];
    isom[0][2] = rot[0][2];
    isom[0][3] = trans[0];
    
    isom[1][0] = rot[1][0];
    isom[1][1] = rot[1][1];
    isom[1][2] = rot[1][2];
    isom[1][3] = trans[1];
    
    isom[2][0] = rot[2][0];
    isom[2][1] = rot[2][1];
    isom[2][2] = rot[2][2];
    isom[2][3] = trans[2];
    
    isom[3][0] = 0;
    isom[3][1] = 0;
    isom[3][2] = 0;
    isom[3][3] = 1;

}

/* Sets its argument to the 4x4 zero matrix (which consists entirely of 0s). */
void mat44Zero(GLdouble m[4][4]){
    int i;
    int j;
    for (i=0;i<4;i++){
        for (j=0;j<4;j++){
            m[i][j]=0;
        }
        
    }
}

/* Multiplies the transpose of the 3x3 matrix m by the 3x1 matrix v. To
clarify, in math notation it computes M^T v. The output CANNOT safely alias the
input. */
void mat331TransposeMultiply(
                             const GLdouble m[3][3], const GLdouble v[3], GLdouble mTimesV[3]){
    mTimesV[0] = (m[0][0]*v[0])+(m[1][0]*v[1])+(m[2][0]*v[2]);
    mTimesV[1] = (m[0][1]*v[0])+(m[1][1]*v[1])+(m[1][2]*v[2]);
    mTimesV[2] = (m[0][2]*v[0])+(m[1][2]*v[1])+(m[2][2]*v[2]);
}


void mat33Invert(const GLdouble m[3][3], GLdouble mInv[3][3]) {
    GLdouble det = -m[2][0]*m[1][1]*m[0][2] + m[1][0]*m[2][1]*m[0][2] + m[2][0]*m[0][1]*m[1][2] - m[0][0]*m[2][1]*m[1][2] -m[1][0]* m[0][1]*m[2][2] +m[0][0]*m[1][1]*m[2][2];
    
    mInv[0][0] = (-m[1][2]*m[2][1] + m[1][1]*m[2][2])/det;
    mInv[1][0] = (m[1][2]* m[2][0] - m[1][0]*m[2][2])/det;
    mInv[2][0] = (-m[1][1] * m[2][0] + m[1][0] * m[2][1])/det;
    mInv[0][1] = (m[0][2] * m[2][1] - m[0][1]*m[2][2])/det;
    mInv[1][1] = (-m[0][2]*m[2][0] + m[0][0]*m[2][2])/det;
    mInv[2][1] = (m[0][1]*m[2][0] - m[0][0]*m[2][1])/det;
    mInv[0][2] = (-m[0][2]*m[1][1] + m[0][1]*m[1][2])/det;
    mInv[1][2] = (m[0][2]*m[1][0] - m[0][0]*m[1][2])/det;
    mInv[2][2] = (-m[0][1]*m[1][0] + m[0][0]*m[1][1])/det;
}

/* Builds a 4x4 matrix for a viewport with lower left (0, 0) and upper right
(width, height). This matrix maps a projected viewing volume
[-1, 1] x [-1, 1] x [-1, 1] to screen [0, w] x [0, h] x [0, 1] (each interval
in that order). */
void mat44Viewport(GLdouble width, GLdouble height, GLdouble view[4][4]){
    GLdouble r = width;
    GLdouble t = height;
    
    view[0][0] = r/2;
    view[0][1] = 0;
    view[0][2] = 0;
    view[0][3] =r/2;
    
    view[1][0] = 0;
    view[1][1] =t/2;
    view[1][2] =0;
    view[1][3] =t/2;
    
    view[2][0] = 0;
    view[2][1] = 0;
    view[2][2] = 0.5;
    view[2][3] = 0.5;
    
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;
    view[3][3] = 1;
}

/* Inverse to mat44Viewport. */
void mat44InverseViewport(GLdouble width, GLdouble height, GLdouble view[4][4]){
    GLdouble r = width;
    GLdouble t = height;
    
    view[0][0] = 2/r;
    view[0][1] = 0;
    view[0][2] = 0;
    view[0][3] = -1 ;
    
    view[1][0] = 0;
    view[1][1] =2/t;
    view[1][2] =0;
    view[1][3] = -1;
    
    view[2][0] = 0;
    view[2][1] = 0;
    view[2][2] = 2;
    view[2][3] = -1;
    
    view[3][0] = 0;
    view[3][1] = 0;
    view[3][2] = 0;
    view[3][3] = 1;
}

void mat41Intersection(int varyDim, GLdouble vary1[], GLdouble vary2[], GLdouble newVary[]) {
    GLdouble vary2Min1[varyDim];
    vecSubtract(varyDim, vary2, vary1, vary2Min1);
    
    GLdouble t;
    t = (vary1[2] + vary1[3])/(vary1[2] + vary1[3] - vary2[2] - vary2[3]);
    
    GLdouble vary2Scaled[varyDim];
    vecScale(varyDim, t, vary2Min1, vary2Scaled);
    
    vecAdd(varyDim, vary1, vary2Scaled, newVary);
}

