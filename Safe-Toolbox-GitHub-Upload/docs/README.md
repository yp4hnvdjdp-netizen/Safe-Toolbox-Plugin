# Safe Toolbox Collaboration Pack

This folder is the shared handoff surface for Codex, Claude, and the human operator.

## How to use

- Keep `current-task.md` as the live task tracker.
- Keep `known-issues.md` as the factual bug list.
- Keep `review-log.md` as the reviewer/implementer exchange log.
- Keep `roadmap.md` as the forward-looking sequence of work.
- Keep `standup-notes.md` as the short daily or session-level status summary.
- Keep `release-handoff.md` as the frozen release reference for review and archive.

## Workflow

1. Update `current-task.md` before starting a new implementation step.
2. Add or update issues in `known-issues.md` when a bug is confirmed.
3. Record review feedback in `review-log.md` after each implementation cycle.
4. Adjust `roadmap.md` only when architecture or sequencing changes.
5. Use `standup-notes.md` for brief progress notes and next actions.
6. Use `release-handoff.md` for the frozen package Claude should review.

## Artifact Expectations

- Build logs
- Logic Pro AU notes
- Screenshots
- Diffs or patch summaries
- Install timestamps
- Asset export status

## Canonical References

- Root release handoff: `../RELEASE_HANDOFF.md`
- Archive guide: `../ARCHIVE_GUIDE.md`
- Install helper: `../scripts/install_au.sh`
