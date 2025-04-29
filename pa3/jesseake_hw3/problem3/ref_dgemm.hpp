#ifndef JESSEAKE_REFDGEMM_HPP
#define JESSEAKE_REFDGEMM_HPP

#include <vector>

void dgemm ( double a , const std :: vector < std :: vector < double > > &A ,
    const std :: vector < std :: vector < double > > &B , double b ,
    std :: vector < std :: vector < double > > & C );

#endif