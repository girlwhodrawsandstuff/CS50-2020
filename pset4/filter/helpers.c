#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (image[i][j].rgbtBlue == image[i][j].rgbtGreen && image[i][j].rgbtBlue == image[i][j].rgbtRed)
            {
                continue;
            }
            int average_color = round(((float)image[i][j].rgbtBlue + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average_color;
            image[i][j].rgbtGreen = average_color;
            image[i][j].rgbtRed = average_color;
        }
    }
    return;
}

// Sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Using the formula provided
            int sepiaRed = round(.393 * (float)image[i][j].rgbtRed + .769 * (float)image[i][j].rgbtGreen + .189 * (float)image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * (float)image[i][j].rgbtRed + .686 * (float)image[i][j].rgbtGreen + .168 *
                                   (float)image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * (float)image[i][j].rgbtRed + .534 * (float)image[i][j].rgbtGreen + .131 * (float)image[i][j].rgbtBlue);

            // Caps the value of r, g, b
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }
            image[i][j].rgbtBlue = sepiaBlue;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtRed = sepiaRed;
        }
    }
    return;
}

// Reflect
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Using temp variable to avoid overwritting
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < height; i++)
    {
        // Cursor variable shows where we are in the array
        int cursor = 0;

        // We will be reading the pixels in reverse order
        // And then rewriting it in the same order
        // Thereby reversing the pixel order
        for (int j = width - 1; j >= 0; j--, cursor++)
        {
            temp[i][cursor] = image[i][j];
        }
    }

    // Copy our values to the final image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp[i][j];
        }
    }


    return;
}

// Blur
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int blurBlue;
    int blurRed;
    int blurGreen;
    float count;
    
    // Using a temp variable to avoid overwriting
    RGBTRIPLE temp[height][width];
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blurBlue = 0;
            blurRed = 0;
            blurGreen = 0;
            count = 0.00;
            for (int k = -1; k < 2; k++)
            {
                if (i + k < 0 || i + k > height - 1)
                {
                    continue;
                }
                for (int h = -1; h < 2; h++)
                {
                    if (j + h < 0 || j + h > width - 1)
                    {
                        continue;
                    }
                    blurBlue += image[i + k][j + h].rgbtBlue;
                    blurRed += image[i + k][j + h].rgbtRed;
                    blurGreen += image[i + k][j + h].rgbtGreen;
                    count++;
                }

            }
            temp[i][j].rgbtBlue = round(blurBlue / count);
            temp[i][j].rgbtRed = round(blurRed / count);
            temp[i][j].rgbtGreen = round(blurGreen / count);
        }
    }

    // Copying the final values
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
    return;
}