scdoc = find_program('scdoc', required: get_option('man-pages'))

if scdoc.found()
  manpages = [
    'labwc-next.1',
    'labwc-next-actions.5',
    'labwc-next-menu.5',
    'labwc-next-philosophy.7',
    'labwc-next-nag.1',
  ]
  foreach manpage : manpages
    markdown = manpage + '.scd'
    custom_target(
      manpage,
      input: markdown,
      output: manpage,
      command: scdoc,
      feed: true,
      capture: true,
      install: true,
      install_dir: join_paths(get_option('mandir'), 'man' + manpage.split('.')[-1])
    )
  endforeach
endif

install_data(
  [
    'autostart',
    'environment',
    'README',
    'shutdown',
    'config.toml',
  ],
  install_dir: get_option('datadir') / 'doc' / meson.project_name()
)
