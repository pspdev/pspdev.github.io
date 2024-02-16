# website

Main repo of the PSPDEV GitHub Organization portal

## Build locally

First, Go here https://jekyllrb.com/docs/installation/ to install all
requirements to build the website locally.

> [!Note]
> Remove the comments of `webrick` and `github-pages` gems in Gemfile so you can build the
website locally.

Run the command to install all gems:

```shell
bundle
```

To access on your browser, run this:
```shell
bundle exec jekyll serve
```

> [!Tip]
> You can access the website to `http://localhost:4000`.

> [!Note]
> You can learn how this website deployed from [here](https://jekyllrb.com/docs/continuous-integration/github-actions/).

## Contributing

If you want to contribute your changes from the website you build locally, just add the comment again on `webrick` and `github-pages` gems in Gemfile so the github action will not get an error. Thanks!
