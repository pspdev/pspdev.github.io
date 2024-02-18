# website

Main repo of the PSPDEV GitHub Organization portal

## Build locally

First, follow the instructions for your system to install the dependencies for building on [this page](https://jekyllrb.com/docs/installation/).

Clone this repo from the terminal:
```shell
git clone https://github.com/pspdev/pspdev.github.io.git
```

Go to the just created directory in the terminal and enter the following commands to install all required gems:

```shell
bundle config set --local path 'vendor/bundle'
bundle install
```

To start the web server run:
```shell
bundle exec jekyll serve
```

You can access the website at `http://localhost:4000`.

> [!Note]
> You can learn how this website deployed in the GitHub pipeline [here](https://jekyllrb.com/docs/continuous-integration/github-actions/).

## Contributing

If you want to contribute your changes feel free to open a pull request [here](https://github.com/pspdev/pspdev.github.io).
