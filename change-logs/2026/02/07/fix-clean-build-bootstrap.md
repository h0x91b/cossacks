Fix clean build support by adding sequential per-project bootstrap build targets, avoiding circular .lib dependency failures when using -t:Rebuild on the whole solution.
