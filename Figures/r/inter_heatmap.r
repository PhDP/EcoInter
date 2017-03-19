library(reshape2)
library(ggplot2)

dat0 <- c(41.39, 40.95, 40.77, 40.45, 40.32, 22.41,
          48.84, 48.76, 49.20, 48.55, 48.28, 32.57,
          53.96, 52.70, 52.46, 51.12, 50.37, 39.28,
          55.54, 55.39, 53.44, 51.95, 50.77, 40.29,
          54.81, 54.79, 53.80, 55.40, 51.40, 40.29,
          54.91, 54.19, 54.27, 54.90, 50.13, 40.17,
          53.15, 53.72, 54.63, 53.68, 50.65, 40.10,
          53.78, 56.23, 56.40, 53.55, 49.97, 40.49,
          53.79, 56.55, 55.13, 53.35, 49.90, 40.76,
          53.24, 55.72, 55.50, 54.54, 49.75, 41.30)

dat <- matrix(dat0, nrow=6, ncol=10)

names(dat) <- paste("X", 1:10)
dat2 <- melt(dat, id.var = "X1")

img <- ggplot(dat2, aes(as.factor(Var1), Var2, group=Var2)) +
  geom_tile(aes(fill = value)) +
  geom_text(aes(fill = dat2$value, label = round(dat2$value, 1))) +
  scale_fill_gradient(low = "white", high = "green")

ggsave(file="heatmap.pdf", plot=img, width=10, height=8)
