// Author: Francesco Vezzani

#ifndef PREPROCESSING_H
#define PREPROCESSING_H

#include <iostream>
#include <enumobjtype.h>
#include <structuserreturn.h>

/**
 * @brief Ask user which value to get
 * @return USER_RETURN One of the possible value of enum `OBJTYPE`
 */
USER_RETURN ask_user();

/**
 * @brief Crop all the images of the selected object
 * @param user_return User return available content
 * return noting 
 */
void crop_model_image(const USER_RETURN user_return);

#endif // PREPROCESSING_H