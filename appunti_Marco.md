# Measurement step

To assess the performance of the program we use:
- mean Intersection over Union (mIoU): 
$$
mIoU = \frac{1}{number\; of\; images\; evaluated} \cdot \frac{Area\; of\; intersection\; of\; two\; boxes}{Area\; of\; union\; of\; two\; boxes}
$$

- Accuracy: number of instances correctly recognized for each object category. Correct prediction if $IoU > 0.5$
